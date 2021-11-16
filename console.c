#include "console.h"

static uint16_t *console_buffer = (uint16_t *)CONSOLE_ADDRESS;

void console_clear(uint8_t fg, uint8_t bg)
{
	int i;

	for (i = 0; i < CONSOLE_BUFSIZE; i++)
		console_buffer[i] = console_entry(0, fg, bg);
}
