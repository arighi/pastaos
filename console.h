#ifndef CONSOLE_H
#define CONSOLE_H

#include "kernel.h"

#define CONSOLE_ADDRESS 0xb8000
#define CONSOLE_BUFSIZE (80 * 25)

enum color {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	GREY,
	DARK_GREY,
	BRIGHT_BLUE,
	BRIGHT_GREEN,
	BRIGHT_CYAN,
	BRIGHT_RED,
	BRIGHT_MAGENTA,
	YELLOW,
	WHITE,

	__COLOR_END__,
};

static inline uint16_t console_entry(unsigned char ch, uint8_t fg, uint8_t bg)
{
	return bg << 12 | fg << 8 | ch;
}

void console_clear(uint8_t fg, uint8_t bg);

#endif /* CONSOLE_H */
