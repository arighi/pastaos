#include <sched.h>
#include <list.h>

#define BUG_ON(x)

/* List of tasks waiting for the CPU */
static LIST_HEAD(task_list);

/* Initial kernel main thread */
struct task_struct init_task = {
	.state = TASK_RUNNING,
};

/* Currently running task */
struct task_struct *current = &init_task;

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
	task->state = TASK_SLEEPING;
	list_add(&task->next, &task_list);
}

static void __attribute__ ((__noinline__))
switch_to(struct task_struct *new, struct task_struct *old)
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

void schedule(void)
{
	struct task_struct *prev, *next;

	prev = current;
	next = list_first_entry_or_null(&task_list, struct task_struct, next);
	if (!next || next == prev)
		return;

	/* Update previously running task */
	switch (prev->state) {
	case TASK_RUNNING:
		prev->state = TASK_SLEEPING;
		list_add_tail(&prev->next, &task_list);
		break;
	default:
		BUG_ON(true);
		return;
	}

	/* Update next running task */
	switch (next->state) {
	case TASK_SLEEPING:
		next->state = TASK_RUNNING;
		list_del_init(&next->next);
		current = next;
		break;
	default:
		BUG_ON(true);
		return;
	}

	/* Context switch */
	switch_to(next, prev);
}
