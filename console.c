#include <console.h>
#include <io.h>

static uint16_t *console_buffer = (unsigned short *)CONSOLE_ADDRESS;
static int console_pos;

static void update_cursor(void)
{
	outb(CONSOLE_PORT + 0, 14);
	outb(CONSOLE_PORT + 1, console_pos >> 8);
	outb(CONSOLE_PORT + 0, 15);
	outb(CONSOLE_PORT + 1, console_pos & 0xff);
}

static void print_char(char c, char color)
{
	switch (c) {
	case '\n':
		console_pos += CONSOLE_WIDTH;
		console_pos -= console_pos % CONSOLE_WIDTH;
		break;
	default:
		console_buffer[console_pos++] = (color << 8 | c);
		break;
	}
	if (console_pos >= 80 * 25)
		console_pos = 0;
}

void printk(const char *str)
{
	char c;

	while ((c = *str++))
		print_char(c, GREY);
	update_cursor();
}
