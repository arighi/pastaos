#ifndef PANIC_H
#define PANIC_H

#include <console.h>

void panic(void);

/* Trigger a kernel bug */
static inline void __kernel_bug(const char *cond, const char *file, int line)
{
	printk("BUG: %s (%s:%d)\n", cond, file, line);
	panic();
}

/* Conditional bug (assert) */
#define BUG_ON(__cond)							\
	do {								\
		if (unlikely(__cond))					\
			__kernel_bug(#__cond, __FILE__, __LINE__);	\
	} while (0)

#endif /* PANIC_H */
