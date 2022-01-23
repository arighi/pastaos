#ifndef SCHED_H
#define SCHED_H

#include <list.h>

/* Default stack size */
#define STACK_SIZE	PAGE_SIZE

/* Declare a kernel thread */
#define DECLARE_TASK(__name, __entry)			\
	struct task_struct __name = {			\
		.entry = __entry,			\
	}

/* Task states */
enum {
	TASK_RUNNING,
	TASK_SLEEPING,
	TASK_ZOMBIE,
};

/* Task structure */
struct task_struct {
	struct list_head next;
	uint32_t stack[STACK_SIZE / sizeof(uint32_t)];
	uint32_t *sp;
	int (*entry)(void);
	int state;
	int ret;
};

/* Current running task */
extern struct task_struct *current;

/* Call the scheduler */
void schedule(void);

/* Create and execute a new task */
int task_run(struct task_struct *task);

#endif /* SCHED_H */
