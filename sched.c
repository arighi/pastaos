#include <sched.h>

struct task_struct init_task = {
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

void task_init(struct task_struct *task, void (*entry)(void))
{
	task->entry = entry;
	stack_init(task);
}

void switch_to(struct task_struct *new, struct task_struct *old)
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
