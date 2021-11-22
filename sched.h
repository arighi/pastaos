#ifndef SCHED_H
#define SCHED_H

struct task_struct {
	void (*entry)(void);
	uint32_t *sp;
};

extern struct task_struct init_task;
extern struct task_struct *current;

void __switch_to(uint32_t *new_stack, uint32_t **old_stack);

static inline void switch_to(struct task_struct *task)
{
	struct task_struct *prev = current;

	current = task;
	__switch_to(task->sp, &prev->sp);
}

int task_run(struct task_struct *task,
	     void (*entry)(void), uint32_t *stack, uint32_t size);

#endif /* SCHED_H */
