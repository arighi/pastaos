#include <kernel.h>
#include <interrupt.h>
#include <console.h>

/* Kernel is dead */
void panic(void)
{
	printk("Kernel panic. Have a nice day.");
	irq_disable();
	for (;;)
		cpu_halt();
}
