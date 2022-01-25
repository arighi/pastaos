#include <types.h>
#include <console.h>
#include <sched.h>

static struct task_struct t1;

static void task1(void)
{
	while (1) {
		printk("hello from task1\n");
		schedule();
	}
}

static struct task_struct t2;

static void task2(void)
{
	while (1) {
		printk("hello from task2\n");
		schedule();
	}
}

static struct task_struct t3;

static void task3(void)
{
	while (1) {
		printk("hello from task3\n");
		schedule();
	}
}

/* kernel main */

int main(int argc __attribute__ ((__unused__)), char **argv __attribute__ ((__unused__)))
{
	task_init(&t1, task1);
	task_init(&t2, task2);
	task_init(&t3, task3);

	while (1) {
		printk("hello from main\n");
		schedule();
	}

	return 0;
}
