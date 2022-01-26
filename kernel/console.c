#include <stdarg.h>
#include <io.h>
#include <console.h>
#include <stdio.h>
#include <stdarg.h>

#define VIDEO_CRT_PORT		0x03d4
#define LAST_LINE (CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1))

static uint16_t *console_buffer = (uint16_t *)CONSOLE_ADDRESS;
int console_pos;

/* Update cursor position on video */
static void refresh_cursor(void)
{
	outb(14, VIDEO_CRT_PORT + 0);
	outb(console_pos >> 8, VIDEO_CRT_PORT + 1);
	outb(15, VIDEO_CRT_PORT + 0);
	outb(console_pos & 0xff, VIDEO_CRT_PORT + 1);
}

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

void kputchar(uint8_t c)
{
	switch (c) {
	case '\n':
		console_pos += CONSOLE_WIDTH;
		console_pos -= console_pos % CONSOLE_WIDTH;
		break;
	case '\r':
		console_pos -= console_pos % CONSOLE_WIDTH;
		break;
	case '\t':
		console_pos += 8;
		break;
	case '\b':
		if (console_pos > 0)
			console_buffer[--console_pos] = console_entry(' ', WHITE, BLACK);
		break;
	default:
		console_buffer[console_pos++] = console_entry(c, GREY, BLACK);
		break;
	}
	if (console_pos >= CONSOLE_BUFSIZE)
		console_scroll_up();
	refresh_cursor();
}

void __console_clear(uint8_t fg, uint8_t bg)
{
	int i;

	console_pos = 0;
	for (i = 0; i < CONSOLE_BUFSIZE; i++)
		console_buffer[i] = console_entry(0, fg, bg);
	refresh_cursor();
}

int printk(const char *fmt, ...)
{
	char buf[1024];
	va_list args;
	unsigned int i;

	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	for (i = 0; i < sizeof(buf); i++) {
		if (!buf[i])
			break;
		kputchar(buf[i]);
	}

	return i;
}
