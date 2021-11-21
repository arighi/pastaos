#ifndef SCHED_H
#define SCHED_H

struct task_struct {
	void (*entry)(void);
	uint32_t *sp;
};

static inline void __stack_init(struct task_struct *t)
{
	*(--t->sp) = (uint32_t)t->entry;	/* eip */
	*(--t->sp) = 0;				/* eax */
	*(--t->sp) = 0;				/* ecx */
	*(--t->sp) = 0;				/* edx */
	*(--t->sp) = 0;				/* ebx */
	*(--t->sp) = 0;				/* ebp */
	*(--t->sp) = 0;				/* esi */
	*(--t->sp) = 0;				/* edi */
}

static inline void
task_init(struct task_struct *t,
	  void (*entry)(void), uint32_t *stack, uint32_t size)
{
	t->entry = entry;
	t->sp = &stack[size];
	__stack_init(t);
}

void __switch_to(uint32_t *new_stack, uint32_t **old_stack);

static inline void switch_to(struct task_struct *next, struct task_struct *prev)
{
	__switch_to(next->sp, &prev->sp);
}

#endif /* SCHED_H */
