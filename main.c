/* types */

#define NULL	0

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

#define CONSOLE_PORT	0x03d4

static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %%al, %%dx" : : "d"(port), "a"(val));
}

static void update_cursor(void)
{
	outb(CONSOLE_PORT + 0, 14);
	outb(CONSOLE_PORT + 1, console_pos >> 8);
	outb(CONSOLE_PORT + 0, 15);
	outb(CONSOLE_PORT + 1, console_pos & 0xff);
}

#define CONSOLE_WIDTH	80
#define CONSOLE_HEIGHT	25

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

/* scheduler */

#define PAGE_SIZE	0x1000
#define STACK_SIZE	PAGE_SIZE

struct task_struct {
	uint32_t stack[STACK_SIZE / sizeof(uint32_t)];
	uint32_t *sp;
	void (*entry)(void);
};

static struct task_struct init_task = {
	.entry = NULL,
};

static void stack_init(struct task_struct *task)
{
	task->sp = &task->stack[STACK_SIZE / sizeof(uint32_t)];
	*(--task->sp) = (uint32_t)task->entry;	/* eip */
	*(--task->sp) = 0;			/* eax */
	*(--task->sp) = 0;			/* ebx */
	*(--task->sp) = 0;			/* ecx */
	*(--task->sp) = 0;			/* edx */
	*(--task->sp) = 0;			/* ebp */
	*(--task->sp) = 0;			/* esi */
	*(--task->sp) = 0;			/* edi */
}

static void task_init(struct task_struct *task, void (*entry)(void))
{
	task->entry = entry;
	stack_init(task);
}

static void switch_to(struct task_struct *new, struct task_struct *old)
{
	register uint32_t old_sp = (uint32_t)&old->sp;
	register uint32_t new_sp = (uint32_t)new->sp;

	asm volatile(
		/* Push general purpose registers */
		"pushl %%eax\n"
		"pushl %%ebx\n"
		"pushl %%ecx\n"
		"pushl %%edx\n"
		"pushl %%ebp\n"
		"pushl %%esi\n"
		"pushl %%edi\n"

		/* Stack switch */
		"mov %%esp, (%1)\n"
		"mov %0, %%esp\n"

		/* Pop general purpose registers */
		"popl %%edi\n"
		"popl %%esi\n"
		"popl %%ebp\n"
		"popl %%edx\n"
		"popl %%ecx\n"
		"popl %%ebx\n"
		"popl %%eax\n"

		/* Return */
		"ret"
		: "=r"(new_sp), "=r"(old_sp) : "r"(new_sp), "r"(old_sp) : "memory"
	);
}

static struct task_struct t1;

static void task1(void)
{
	while (1) {
		printk("hello from task1\n");
		switch_to(&init_task, &t1);
	}
}

/* kernel main */

static struct task_struct init_task;

int main(int argc __attribute__ ((__unused__)), char **argv __attribute__ ((__unused__)))
{
	task_init(&t1, task1);

	while (1) {
		printk("hello from main\n");
		switch_to(&t1, &init_task);
	}

	return 0;
}
