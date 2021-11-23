#include <kernel.h>
#include <console.h>
#include <sched.h>
#include <interrupt.h>
#include <panic.h>

static int task1(void)
{
	while (1) {
		console_putchar('B');
		cpu_halt();
		switch_to(&init_task);
	}
	return 0;
}

static DECLARE_TASK(t1, task1);

int main(void)
{
	task_run(&t1);

	while (1) {
		console_putchar('A');
		cpu_halt();
		switch_to(&t1);
	}
	return 0;
}
