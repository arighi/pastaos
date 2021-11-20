#include "console.h"

#define LAST_LINE (CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1))

static uint16_t *console_buffer = (uint16_t *)CONSOLE_ADDRESS;
int console_pos;

static void console_scroll_up(void)
{
	int i;

	/* Scroll up */
	for (i = 0; i < LAST_LINE; i++)
		console_buffer[i] = console_buffer[i + CONSOLE_WIDTH];
	/* Blank bottom line */
	for (i = 0; i < CONSOLE_WIDTH; i++)
		console_buffer[LAST_LINE + i] = console_entry(0, WHITE, BLACK);
	/* Update cursor position */
	console_pos -= CONSOLE_WIDTH;
}

void console_putchar(uint8_t c)
{
	console_buffer[console_pos++] = console_entry(c, GREY, BLACK);
	if (console_pos >= CONSOLE_BUFSIZE)
		console_scroll_up();
}

void __console_clear(uint8_t fg, uint8_t bg)
{
	int i;

	console_pos = 0;
	for (i = 0; i < CONSOLE_BUFSIZE; i++)
		console_buffer[i] = console_entry(0, fg, bg);
}
