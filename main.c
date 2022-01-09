/* types */

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

/* console driver */

#define CONSOLE_ADDRESS 0xb8000

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

static uint16_t *console_buffer = (unsigned short *)CONSOLE_ADDRESS;
static int console_pos;

static void print_char(char c, char color)
{
	console_buffer[console_pos++] = (color << 8 | c);
	if (console_pos >= 80 * 25)
		console_pos = 0;
}

static void printk(const char *str)
{
	char c;

	while ((c = *str++))
		print_char(c, GREY);
}

/* kernel main */

int main(int argc, char **argv)
{
	if (argc == 1)
		printk("hello world");
	return 0;
}
