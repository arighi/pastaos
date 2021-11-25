#include <kernel.h>
#include <interrupt.h>
#include <console.h>

/* Kernel is dead */
void panic(void)
{
	__console_clear(RED, RED);
	irq_disable();
	for (;;)
		cpu_halt();
}
