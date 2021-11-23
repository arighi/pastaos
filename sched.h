#ifndef SCHED_H
#define SCHED_H

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
	uint32_t stack[STACK_SIZE];
	uint32_t *sp;
	int (*entry)(void);
	int state;
	int ret;
};

/* Main kernel task structure */
extern struct task_struct init_task;

/* Current running task */
extern struct task_struct *current;

/* Context switch to a different task */
int switch_to(struct task_struct *task);

/*
 * Initialize a task with custom parameters (e.g., different stack size, or a
 * different entry point).
 */
void task_init(struct task_struct *t,
	       int (*entry)(void), uint32_t *stack, uint32_t size);

/* Create and execute a new task */
int task_run(struct task_struct *task);

#endif /* SCHED_H */
