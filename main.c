#include <kernel.h>
#include <console.h>
#include <sched.h>
#include <interrupt.h>
#include <panic.h>

static struct task_struct t1;
static uint32_t s1[1024];

static int task1(void)
{
	while (1) {
		console_putchar('B');
		cpu_halt();
		switch_to(&init_task);
	}
	return 0;
}

int main(void)
{
	task_run(&t1, task1, s1, ARRAY_SIZE(s1));

	while (1) {
		console_putchar('A');
		cpu_halt();
		switch_to(&t1);
	}
	return 0;
}
