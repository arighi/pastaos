#include <kernel.h>
#include <interrupt.h>
#include <console.h>

static void __init__ init(void)
{
	interrupt_init();
}

int main(void)
{
	int i;

	init();

	console_clear();
	for (i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++)
		console_putchar('A');
	for (i = 0; i < CONSOLE_WIDTH; i++)
		console_putchar('B');
	return 0;
}
