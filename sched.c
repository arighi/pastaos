#include <kernel.h>
#include <sched.h>

/* Dummy entry point for init task */
static void __init_task_main(void)
{
}

/* Initial kernel thread */
struct task_struct init_task = {
	.entry = __init_task_main,
};

/* Current running thread */
struct task_struct *current = &init_task;

/* Low-level context switch routine to perform a stack switch */
void __attribute__ ((__noinline__))
__switch_to(uint32_t *new_stack, uint32_t **old_stack)
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

/* Switch to a different task (if possible) */
int switch_to(struct task_struct *task)
{
	struct task_struct *prev = current;

	/* Check if target task is zombie */
	if (task->entry == NULL)
		return -EINVAL;

	current = task;
	__switch_to(task->sp, &prev->sp);
	return 0;
}

/* Task is exiting: clean-up routine */
static void __exit_point(void)
{
	/* Set current task to zombie by cleaning its entry point */
	current->entry = NULL;
	switch_to(&init_task);
}

/* Initialize the stack for a new task */
static void __stack_init(struct task_struct *t)
{
	*(--t->sp) = (uint32_t)__exit_point;	/* ret when task is done */
	*(--t->sp) = (uint32_t)t->entry;	/* eip */
	*(--t->sp) = 0;				/* eax */
	*(--t->sp) = 0;				/* ecx */
	*(--t->sp) = 0;				/* edx */
	*(--t->sp) = 0;				/* ebx */
	*(--t->sp) = 0;				/* ebp */
	*(--t->sp) = 0;				/* esi */
	*(--t->sp) = 0;				/* edi */
}

/* Initialize a new task structure */
static void
task_init(struct task_struct *t,
	  void (*entry)(void), uint32_t *stack, uint32_t size)
{
	t->entry = entry;
	t->sp = &stack[size];
	__stack_init(t);
}

/* Start a new task */
int task_run(struct task_struct *task,
	     void (*entry)(void), uint32_t *stack, uint32_t size)
{
	task_init(task, entry, stack, size);
	switch_to(task);

	return 0;
}
