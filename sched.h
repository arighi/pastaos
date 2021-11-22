#ifndef SCHED_H
#define SCHED_H

struct task_struct {
	void (*entry)(void);
	uint32_t *sp;
};

extern struct task_struct init_task;
extern struct task_struct *current;

int switch_to(struct task_struct *task);

int task_run(struct task_struct *task,
	     void (*entry)(void), uint32_t *stack, uint32_t size);

#endif /* SCHED_H */
