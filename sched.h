#ifndef SCHED_H
#define SCHED_H

/* Task states */
enum {
	TASK_RUNNING,
	TASK_SLEEPING,
	TASK_ZOMBIE,
};

/* Task structure */
struct task_struct {
	void (*entry)(void);
	uint32_t *sp;
	int state;
};

/* Main kernel task structure */
extern struct task_struct init_task;

/* Current running task */
extern struct task_struct *current;

/* Context switch to a different task */
int switch_to(struct task_struct *task);

/* Create and execute a new task */
int task_run(struct task_struct *task,
	     void (*entry)(void), uint32_t *stack, uint32_t size);

#endif /* SCHED_H */
