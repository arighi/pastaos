#include <interrupt.h>
#include <clock.h>
#include <console.h>

void __init__ init(void)
{
	console_init();
	clock_init();
	interrupt_init();
}
