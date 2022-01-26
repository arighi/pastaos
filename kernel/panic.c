#include <kernel.h>
#include <interrupt.h>
#include <console.h>

/* Kernel is dead */
void __noreturn panic(void)
{
	printk(">>> Kernel panic. Have a nice day. <<<\n");
	irq_disable();
	for (;;)
		cpu_halt();
}
