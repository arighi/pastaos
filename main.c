#include <kernel.h>
#include <console.h>

int main(void)
{
	int i;

	for (i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++)
		console_putchar('A');
	for (i = 0; i < CONSOLE_WIDTH; i++)
		console_putchar('B');
	return 0;
}
