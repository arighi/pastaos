#include <interrupt.h>
#include <console.h>

void __init__ init(void)
{
	interrupt_init();
	console_init();
}
