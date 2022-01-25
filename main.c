#include <types.h>
#include <console.h>
#include <sched.h>

static struct task_struct t1;

static void task1(void)
{
	while (1) {
		printk("hello from task1\n");
		switch_to(&init_task, &t1);
	}
}

/* kernel main */

int main(int argc __attribute__ ((__unused__)), char **argv __attribute__ ((__unused__)))
{
	task_init(&t1, task1);

	while (1) {
		printk("hello from main\n");
		switch_to(&t1, &init_task);
	}

	return 0;
}
