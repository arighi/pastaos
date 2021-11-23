#include <interrupt.h>
#include <clock.h>
#include <console.h>

void __init__ init(void)
{
	console_init();
	interrupt_init();
	clock_init();
}
