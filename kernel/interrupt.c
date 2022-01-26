#include <kernel.h>
#include <console.h>
#include <panic.h>
#include <interrupt.h>

/* PIC 8259 master */
#define PORT_8259_M		0x20

/* PIC 8259 slave */
#define PORT_8259_S		0xa0

/* PIC 8259 master (interrupt mask) */
#define PORT_INT_MASK_M		0x21

/* PIC 8259 slave (interrupt mask) */
#define PORT_INT_MASK_S		0xa1

/* PIC 8259 EOI (End Of Interrupt) */
#define EOI			0x20

/* Total amount of supported interrupts */
#define IRQ_NR			16

/* Number of entries in the IDT */
#define IDT_SIZE			256

/* The PIC irq mask */
static uint16_t irq_mask = 0xffff;

/* Store bits of all the received spurious IRQs */
static int spurious_irq_mask;

/* The PIC irq mask for the master controller */
#define irq_master_mask         (((uint8_t *)&(irq_mask))[0])

/* The PIC irq mask for the slave controller */
#define irq_slave_mask          (((uint8_t *)&(irq_mask))[1])

/* IDT entry */
typedef struct idt_entry {
	uint16_t offset0_15;
	uint16_t selector;
	uint8_t paramcnt;
	uint8_t attribs;
	uint16_t offset16_31;
} __attribute__ ((__packed__)) idt_entry_t;

/* IDT register */
typedef struct idt_reg {
        uint16_t limit;
        uint8_t base[sizeof(uint32_t)];
} __attribute__ ((__packed__)) idt_reg_t;

/* Interrupt Descriptor Table (IDT) */
volatile idt_entry_t idt[IDT_SIZE];

/* Pointer to the Interrupt Descriptor Table Register */
volatile idt_reg_t idt_ptr;

/* IRQ descriptor */
struct irq_desc {
	void (*handler)(irq_context_t *);
} __attribute__ ((__packed__));

static void unhandled_interrupt(irq_context_t __attribute__ ((__unused__)) *c)
{
	BUG_ON(true);
}

static struct irq_desc irq_handler[IRQ_NR] __attribute__ ((__aligned__(16))) = {
        [0 ... IRQ_NR - 1] = {
		.handler = unhandled_interrupt,
	}
};

void default_exc_handler(exception_context_t *c)
{
	printk("exception: %d\n", c->exc);
	BUG_ON(true);
}

/* Check if the IRQ occurred can be considered spurious or real */
static int irq_real(int irq)
{
	int value;
	int irqmask = 1 << irq;

	if (irq < 8) {
		out(PORT_8259_M, 0x0B);
		value = in(PORT_8259_M) & irqmask;
		out(PORT_8259_M, 0x0A);

		return value;
	}
	out(PORT_8259_S, 0x0B);
	value = in(PORT_8259_S) & (irqmask >> 8);
	out(PORT_8259_S, 0x0A);

	return value;
}

/* Handle a spurious (unexpected) IRQ */
static void handle_spurious_irq(int irq)
{
	unsigned int mask = 1 << irq;

	if (irq_real(irq))
		return;
	/*
	 * At this point we can be sure the IRQ is spurious,
	 * lets ACK and report it. [once per IRQ]
	 */
	if (!(spurious_irq_mask & mask)) {
		/* TODO: notify about spurious IRQ */
		spurious_irq_mask |= mask;
	}
}

/* Acknowledge the irq line */
static void mask_and_ack_irq(int irq)
{
	unsigned int mask = 1 << irq;
	uint32_t flags;

	irq_save(flags);

	/*
	 * Spurious IRQ detection. Remember that spurious IRQs are usually sign
	 * of hardware problems, so we want to do the strictly needed checks
	 * without slowing down good hardware unnecessarily.
	 *
	 * Note: IRQ7 and IRQ15 (the two spurious IRQs usually resulting from
	 * the 8259A-1|2 PICs) seems to occur even if the IRQ is masked in the
	 * 8259A.
	 */
	if (irq_mask & mask)
		handle_spurious_irq(irq);

	/* Mask the irq line */
	irq_mask |= mask;

	if (irq & 8) {
		/* Dummy operation (required) */
		(void)in(PORT_INT_MASK_S);

		/* Mask the irq on the slave */
		out(PORT_INT_MASK_S, irq_slave_mask);
		out(PORT_8259_S, 0x60 + (irq & 7));
		out(PORT_8259_S, 0x60 + 2);

		/* Acknowledge the irq sending a EOI */
		out(PORT_8259_S, EOI);
		out(PORT_8259_M, EOI);
	} else {
		/* Dummy operation (required) */
		(void)in(PORT_INT_MASK_M);

		/* Mask the irq on the master */
		out(PORT_INT_MASK_M, irq_master_mask);
		out(PORT_8259_M, 0x60 + irq);

		/* Acknowledge the irq sending a EOI */
		out(PORT_8259_M, EOI);
	}

	irq_restore(flags);
}

/* Reprogram PIC to disable an irq line */
static void pic_disable_irq(uint8_t irq)
{
        unsigned int mask = 1 << irq;
        uint32_t flags;

        irq_save(flags);
        irq_mask |= mask;
        if (irq & 8)
                out(PORT_INT_MASK_S, irq_slave_mask);
        else
                out(PORT_INT_MASK_M, irq_master_mask);
        irq_restore(flags);
}

/* Reprogram PIC to enable an irq line */
static void pic_enable_irq(uint8_t irq)
{
	unsigned int mask = ~(1 << irq);
	uint32_t flags;

	irq_save(flags);
	irq_mask &= mask;
	if (irq & 8)
		out(PORT_INT_MASK_S, irq_slave_mask);
	else
		out(PORT_INT_MASK_M, irq_master_mask);
	irq_restore(flags);
}

/* End of interrupt routine: re-enable IRQ line in the PIC */
void end_of_irq(irq_context_t *c)
{
	pic_enable_irq(c->irq);
}

/* Initialize the Programmable Interrupt Controllers (PICs) */
static void irq_init(void)
{
        /* Start initialization for master 8259a controller */
        out(PORT_8259_M, 0x11);
        /* Master base vector [ 0x20 ... 0x27 ] */
        out(PORT_INT_MASK_M, 0x20);
        /* IRQ2 cascade to slave */
        out(PORT_INT_MASK_M, 1 << 2);
        /* Finish master 8259a initialization (manual EOI) */
        out(PORT_INT_MASK_M, 0x01);

        /* Start initialization for slave 8259a controller */
        out(PORT_8259_S, 0x11);
        /* Slave base vector [ 0x28 ... 0x2f ] */
        out(PORT_INT_MASK_S, 0x28);
        /* Cascade on IRQ2 */
        out(PORT_INT_MASK_S, 2);
        /* Finish slave 8259a initialization */
        out(PORT_INT_MASK_S, 0x01);

        /* Initialize the irq mask for master 8259a */
        irq_mask &= ~(1 << 2);
        out(PORT_INT_MASK_M, irq_master_mask);

        /* Initialize the irq mask for master 8259a */
        out(PORT_INT_MASK_S, irq_slave_mask);
}

/* Default interrupt routine */
void default_irq_handler(irq_context_t *c)
{
	mask_and_ack_irq(c->irq);
        if (c->irq < IRQ_NR)
                irq_handler[c->irq].handler(c);
        else
                unhandled_interrupt(c);
	end_of_irq(c);
}

/* Set up an IDT entry */
static void
setup_idt_entry(uint8_t i, uint16_t selector, uint32_t offset, uint8_t attribs, uint8_t paramcnt)
{
	uint32_t flags;

	irq_save(flags);

	idt[i].offset0_15 =  offset & 0xFFFF;
	idt[i].offset16_31 = offset >> 16;
	idt[i].selector = selector;
	idt[i].attribs = attribs;
	idt[i].paramcnt = paramcnt;

	irq_restore(flags);
}

/* Setup an interrupt gate */
static inline void set_intr_gate(unsigned int i, void *addr)
{
	setup_idt_entry(i, KERNEL_CODE, (uint32_t)addr, INT_GATE, 0);
}

/* Setup an exception gate */
static inline void set_trap_gate(unsigned int i, void *addr)
{
	setup_idt_entry(i, KERNEL_CODE, (uint32_t)addr, TRAP_GATE, 0);
}

/* IRQ hooks defined in assembly */
extern void _irq_00(void);
extern void _irq_01(void);
extern void _irq_02(void);
extern void _irq_03(void);
extern void _irq_04(void);
extern void _irq_05(void);
extern void _irq_06(void);
extern void _irq_07(void);
extern void _irq_08(void);
extern void _irq_09(void);
extern void _irq_0A(void);
extern void _irq_0B(void);
extern void _irq_0C(void);
extern void _irq_0D(void);
extern void _irq_0E(void);
extern void _irq_0F(void);
extern void _irq_unhand(void);

/* Exception hooks defined in assembly */
extern void _exc_00(void);
extern void _exc_01(void);
extern void _exc_02(void);
extern void _exc_03(void);
extern void _exc_04(void);
extern void _exc_05(void);
extern void _exc_06(void);
extern void _exc_07(void);
extern void _exc_08(void);
extern void _exc_09(void);
extern void _exc_0A(void);
extern void _exc_0B(void);
extern void _exc_0C(void);
extern void _exc_0D(void);
extern void _exc_0E(void);
extern void _exc_0F(void);
extern void _exc_unhand(void);

/* Initialize the IDT (Interrupt Descriptor Table) */
static void idt_init(void)
{
	int i;

	/* Initialize exception handlers (0x00 ... 0x1f) */
	for (i = 0x00; i <= 0x1F; i++)
		set_intr_gate(i, &_exc_unhand);

	/* Setup the IDT entries for exceptions */

	set_trap_gate(0x00, &_exc_00); /* divide error */
	set_trap_gate(0x01, &_exc_01); /* debug */
	set_intr_gate(0x02, &_exc_02); /* nmi */
	set_trap_gate(0x03, &_exc_03); /* int3 */
	set_trap_gate(0x04, &_exc_04); /* overflow */
	set_trap_gate(0x05, &_exc_05); /* bounds */
	set_trap_gate(0x06, &_exc_06); /* invalid operation */
	set_trap_gate(0x07, &_exc_07); /* device not available */
	set_trap_gate(0x08, &_exc_08); /* double fault */
	set_trap_gate(0x09, &_exc_09); /* coprocessor segment overrun */
	set_trap_gate(0x0a, &_exc_0A); /* invalid tss */
	set_trap_gate(0x0b, &_exc_0B); /* segment not present */
	set_trap_gate(0x0c, &_exc_0C); /* stack fault */
	set_trap_gate(0x0d, &_exc_0D); /* general protection */
	set_intr_gate(0x0e, &_exc_0E); /* page fault */
	set_intr_gate(0x0f, &_exc_0F); /* spurious interrupt */

	/* Initialize interrupt handlers (0x20 ... 0xff) */
	for (i = 0x20; i <= 0xff; i++)
		set_intr_gate(i, &_irq_unhand);

	/* Setup the IDT entries for IRQs */
	set_trap_gate(0x20, &_irq_00);
	set_trap_gate(0x21, &_irq_01);
	set_trap_gate(0x22, &_irq_02);
	set_trap_gate(0x23, &_irq_03);
	set_trap_gate(0x24, &_irq_04);
	set_trap_gate(0x25, &_irq_05);
	set_trap_gate(0x26, &_irq_06);
	set_trap_gate(0x27, &_irq_07);
	set_trap_gate(0x28, &_irq_08);
	set_trap_gate(0x29, &_irq_09);
	set_trap_gate(0x2a, &_irq_0A);
	set_trap_gate(0x2b, &_irq_0B);
	set_trap_gate(0x2c, &_irq_0C);
	set_trap_gate(0x2d, &_irq_0D);
	set_trap_gate(0x2e, &_irq_0E);
	set_trap_gate(0x2f, &_irq_0F);

	/* Set up the IDT pointer */
	idt_ptr.limit = IDT_SIZE * sizeof(idt_entry_t) - 1;
	*(uint32_t *)idt_ptr.base = (uint32_t)&idt;

	/* Load info into IDTR register */
	__asm__ __volatile__ ("lidtl (%0)" : : "r"((uint32_t)&idt_ptr));
}

/* Uninstall an interrupt handler */
int uninstall_interrupt_handler(int irq)
{
        uint32_t flags;

        if (irq > IRQ_NR)
                return -EINVAL;

        irq_save(flags);
	pic_disable_irq(irq);
        irq_handler[irq].handler = NULL;
        irq_restore(flags);

	return 0;
}

/* Install an interrupt handler */
int install_interrupt_handler(int irq, void *handler, int type)
{
        uint32_t flags;

        if (irq > IRQ_NR)
                return -EINVAL;

        irq_save(flags);
        irq_handler[irq].handler = handler;
        idt[0x20 + irq].attribs = type;
        pic_enable_irq(irq);
        irq_restore(flags);

	return 0;
}

void __init__ interrupt_init(void)
{
	idt_init();
	irq_init();

	/* Enable interrupts */
	irq_enable();
}
