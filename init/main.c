#include <kernel.h>
#include <console.h>
#include <sched.h>
#include <interrupt.h>
#include <panic.h>

#define TASK(__n, __worker)				\
	static int __noreturn __task ## __n(void)	\
	{						\
		while (true)				\
			__worker(__n);			\
	}						\
	static DECLARE_TASK(task ## __n, __task ## __n)	\

static void worker(int pid)
{
	printk("hello from pid %d\n", pid);
	cpu_halt();
	schedule();
}

TASK(1, worker);
TASK(2, worker);
TASK(3, worker);
TASK(4, worker);
TASK(5, worker);
TASK(6, worker);
TASK(7, worker);
TASK(8, worker);
TASK(9, worker);

int main(void)
{
	task_run(&task1);
	task_run(&task2);
	task_run(&task3);
	task_run(&task4);
	task_run(&task5);
	task_run(&task6);
	task_run(&task7);
	task_run(&task8);
	task_run(&task9);

	while (true)
		worker(0);

	return 0;
}
