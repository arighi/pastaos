#include <interrupt.h>

/* Kernel timer clock frequency */
#define HZ		100

/* Clock driver definitions */
#define CLOCK_IRQ		0x00		/* Kernel timer IRQ line */
#define LATCH_COUNT		0x00		/* To copy chip count */
#define TIMER0			0x40		/* I/O port for timer channel 0 */
#define TIMER2			0x42		/* I/O port for timer channel 2 */
#define TIMER_MODE		0x43		/* I/O port for timer mode control */
#define SQUARE_WAVE		0x36		/* The sqare-wave form */
#define CLOCK_FREQ		1193182L	/* Clock frequency for timer in PC */

/* Value to initialize timer */
#define CLOCK_COUNT     (CLOCK_FREQ / HZ)

/* LATCH is used for the interval timer */
#define LATCH		((CLOCK_FREQ + HZ / 2) / HZ)

static void clock_handler(irq_context_t *c)
{
	end_of_irq(c);
}

void __init__ clock_init(void)
{
        uint32_t flags;

        irq_save(flags);

        /* Set timer on */
        out(TIMER_MODE, SQUARE_WAVE);
        /* Timer low byte */
        out(TIMER0, CLOCK_COUNT & 0xff);
        /* Timer high byte */
        out(TIMER0, CLOCK_COUNT >> 8);

        irq_restore(flags);

        install_interrupt_handler(CLOCK_IRQ, clock_handler, TRAP_GATE);
}
