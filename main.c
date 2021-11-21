#include <kernel.h>
#include <console.h>
#include <sched.h>
#include <interrupt.h>

#define STACK_SIZE	1024

static struct task_struct main_task;

static struct task_struct t1;
static uint32_t s1[STACK_SIZE];

static void task1(void)
{
	while (1) {
		console_putchar('B');
		switch_to(&main_task, &t1);
	}
}

int main(void)
{
	task_init(&t1, task1, s1, ARRAY_SIZE(s1));

	while (1) {
		console_putchar('A');
		switch_to(&t1, &main_task);
	}

	return 0;
}
