#ifndef SCHED_H
#define SCHED_H

#include <types.h>
#include <list.h>

#define PAGE_SIZE	0x1000
#define STACK_SIZE	PAGE_SIZE

/* Task states */
enum {
	TASK_RUNNING,
	TASK_SLEEPING,
};

struct task_struct {
	uint32_t stack[STACK_SIZE / sizeof(uint32_t)];
	uint32_t *sp;
	void (*entry)(void);
	int state;
	struct list_head next;
};

extern struct task_struct init_task;

void task_init(struct task_struct *task, void (*entry)(void));
void schedule(void);

#endif /* SCHED_H */
