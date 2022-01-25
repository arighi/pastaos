#ifndef SCHED_H
#define SCHED_H

#include <types.h>

#define PAGE_SIZE	0x1000
#define STACK_SIZE	PAGE_SIZE

struct task_struct {
	uint32_t stack[STACK_SIZE / sizeof(uint32_t)];
	uint32_t *sp;
	void (*entry)(void);
};

extern struct task_struct init_task;

void task_init(struct task_struct *task, void (*entry)(void));
void switch_to(struct task_struct *new, struct task_struct *old);

#endif /* SCHED_H */
