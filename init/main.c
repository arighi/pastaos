#include <kernel.h>
#include <console.h>
#include <sched.h>
#include <interrupt.h>
#include <panic.h>

static int task1(void)
{
	while (1) {
		printk("%s\n", "hello from task1");
		cpu_halt();
		schedule();
	}
	return 0;
}

static DECLARE_TASK(t1, task1);

int main(void)
{
	task_run(&t1);

	while (1) {
		printk("%s\n", "hello from init");
		cpu_halt();
		schedule();
	}
	return 0;
}
