#include <kernel.h>
#include <panic.h>
#include <interrupt.h>
#include <sched.h>

/* List of tasks */
static LIST_HEAD(task_list);

/* Initial kernel thread */
struct task_struct init_task = {
	.state = TASK_RUNNING,
};

/* Current running thread */
struct task_struct *current = &init_task;

/* Low-level context switch routine to perform a stack switch */
static void __attribute__ ((__noinline__))
switch_to(uint32_t *new_stack, uint32_t **old_stack)
{
	__asm__ __volatile__(
		     "pushl %%eax\n"
		     "pushl %%ecx\n"
		     "pushl %%edx\n"
		     "pushl %%ebx\n"
		     "pushl %%ebp\n"
		     "pushl %%esi\n"
		     "pushl %%edi\n"
		     "mov %%esp, (%1)\n"
		     "mov %0, %%esp\n"
		     "popl %%edi\n"
		     "popl %%esi\n"
		     "popl %%ebp\n"
		     "popl %%ebx\n"
		     "popl %%edx\n"
		     "popl %%ecx\n"
		     "popl %%eax\n"
		     "ret" : "=r"(new_stack), "=r"(old_stack) :
		              "r"(new_stack), "r"(old_stack) : "memory");
}

/* Main scheduler function */
void schedule(void)
{
	struct task_struct *prev, *next;

	/*
	 * Pop the next task from the task list, if there's a different task to
	 * run.
	 */
	prev = current;
	next = list_first_entry_or_null(&task_list, struct task_struct, next);
	if (!next)
		return;
	BUG_ON(next == prev);

	/* Update current task state */
	switch (prev->state) {
	case TASK_RUNNING:
		current->state = TASK_SLEEPING;
		list_add_tail(&current->next, &task_list);
		break;
	case TASK_ZOMBIE:
		break;
	case TASK_SLEEPING:
		BUG_ON(true);
		break;
	default:
		printk("Invalid task state: %d\n", prev->state);
		BUG_ON(true);
		break;
	}

	/* Update next task state */
	switch (next->state) {
	case TASK_RUNNING:
		BUG_ON(true);
		break;
	case TASK_SLEEPING:
		next->state = TASK_RUNNING;
		list_del_init(&next->next);
		current = next;
		break;
	case TASK_ZOMBIE:
		BUG_ON(true);
		break;
	default:
		printk("Invalid task state: %d\n", next->state);
		BUG_ON(true);
		break;
	}

	/* Switch to the next task */
	switch_to(next->sp, &prev->sp);
}

/* Task is exiting: clean-up routine */
static void exit_point(void)
{
	register int ret asm("eax");

	/* Return code is passed using register eax according to the ABI */
	current->ret = ret;

	/* Set current task to zombie by cleaning its entry point */
	current->state = TASK_ZOMBIE;
	schedule();
}

/* Initialize the stack for a new task */
static void stack_init(struct task_struct *task)
{
	*(--task->sp) = (uint32_t)exit_point;	/* ret when task is done */
	*(--task->sp) = (uint32_t)task->entry;	/* eip */
	*(--task->sp) = 0;			/* eax */
	*(--task->sp) = 0;			/* ecx */
	*(--task->sp) = 0;			/* edx */
	*(--task->sp) = 0;			/* ebx */
	*(--task->sp) = 0;			/* ebp */
	*(--task->sp) = 0;			/* esi */
	*(--task->sp) = 0;			/* edi */
}

/* Initialize a new task structure */
static void task_init(struct task_struct *task,
		      int (*entry)(void), uint32_t *stack, uint32_t size)
{
	/* Initialize task entry point */
	task->entry = entry;

	/* Initialize task stack */
	task->sp = &stack[size / sizeof(stack[0])];
	stack_init(task);

	/* Add task to the task list as sleeping */
	task->state = TASK_SLEEPING;
	list_add(&task->next, &task_list);
}

/* Start a new task */
int task_run(struct task_struct *task)
{
	/* Initialize task */
	task_init(task, task->entry, task->stack, STACK_SIZE);

	/* Call the scheduler */
	schedule();

	return 0;
}
