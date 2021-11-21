#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <kernel.h>

/* Save EFLAGS register */
#define save_flags(flags)	\
	__asm__ __volatile__ ("pushfl ; popl %0" : "=g"(flags) : )

/* Restore EFLAGS register */
#define restore_flags(flags)	\
	__asm__ __volatile__ ("pushl %0 ; popfl" : : "g"(flags) : "memory", "cc")

/* Save EFLAGS and disable interrupts */
#define irq_save(flags)		\
	__asm__ __volatile__ ("pushfl ; popl %0 ; cli" : "=g"(flags) : : "memory")

/* Restore EFLAGS and enable interrupts */
#define irq_restore(flags)	restore_flags(flags)

/* Disable all interrrupts */
static inline void irq_disable(void)
{
	__asm__ __volatile__("cli": : :"memory");
}

/* Enable all interrupts */
static inline void irq_enable(void)
{
	__asm__ __volatile__("sti": : :"memory");
}

/* Halt the CPU and wait for the next interrupt */
static inline void cpu_halt(void)
{
        __asm__ __volatile__("hlt" : : :"memory");
}

/* Interrupt context in the stack */
typedef struct irq_context {
	/* General purpose register */
	uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
	/* Segment register */
	uint32_t es, ds;
	/* The interrupt number */
	uint32_t irq;
	/* Instruction pointer */
	uint32_t eip;
	/* Code segment register */
	uint32_t cs;
	/* EFLAGS register */
	uint32_t eflags;
} __attribute__ ((packed)) irq_context_t;

/* Exception context in the stack */
typedef struct exception_context
{
	/* General purpose register */
	uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
	/* Segment register */
	uint32_t es, ds;
	/* The exception number and error code */
	uint32_t exc, err_code;
	/* Instruction pointer */
	uint32_t eip;
	/* Code segment register */
	uint32_t cs;
	/* EFLAGS register */
	uint32_t eflags;
} __attribute__ ((packed)) exception_context_t;

static inline uint8_t in(uint16_t port)
{
	register uint8_t val;
	__asm__ __volatile__ ("inb %%dx, %%al" : "=a" (val) : "d" (port));
        return val;
}

static inline uint16_t in16(uint16_t port)
{
	register uint16_t val;
	__asm__ __volatile__ ("inw %%dx, %%ax" : "=a" (val) : "d" (port));
        return val;
}

static inline uint32_t in32(uint16_t port)
{
	register uint32_t val;
	__asm__ __volatile__ ("inl %%dx, %%eax" : "=a" (val) : "d" (port));
        return val;
}

static inline void out(uint16_t port, uint8_t val)
{
	__asm__ __volatile__ ("outb %%al, %%dx" : : "d" (port), "a" (val));
}

static inline void out16(uint16_t port, uint16_t val)
{
	__asm__ __volatile__ ("outw %%ax, %%dx" : : "d" (port), "a" (val));
}

static inline void out32(uint16_t port, uint32_t val)
{
	__asm__ __volatile__ ("outl %%eax, %%dx" : : "d" (port), "a" (val));
}

void __init__ interrupt_init(void);

#endif /* INTERRUPT_H */
