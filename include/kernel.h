#ifndef KERNEL_H
#define KERNEL_H

#include <errno.h>
#include <types.h>
#include <compiler.h>

/* Branch hints */
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

/* Function that never returns */
#define __noreturn	__attribute__((__noreturn__))

/* Amount of items in array */
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(*(x)))

/* Get the offset of a member inside a struct */
#define offsetof(__s, __m)	((size_t)&((__s *)0)->__m)

/* Get the object that contains a specific member */
#define container_of(__ptr, __type, __member) \
	((__type *)(void *)((char *)(__ptr) - offsetof(__type, __member)))

/*
 * Memory barrier
 *
 * Force strict CPU ordering (required to preserve the order of execution).
 */
#define barrier() \
        __asm__ __volatile__ ("lock; addl $0,0(%%esp)": : :"memory")

/* Prevent compiler from reordering reads or writes when READ_ONCE() is used */
#define READ_ONCE(x) (*(const volatile typeof(x) *)&(x))

/* Prevent compiler from reordering reads or writes when WRITE_ONCE() is used */
#define WRITE_ONCE(x, val)					\
	do {							\
		*(volatile typeof(x) *)&(x) = (val);		\
	} while (0)

/* Write to memory safely (prevent CPU reordering) */
#define smp_store_release(p, v)					\
do {								\
        barrier();						\
        WRITE_ONCE(*p, v);					\
} while (0)

/* Read from memory safely (prevent CPU reordering) */
#define smp_load_acquire(p)					\
({								\
        typeof(*p) ___p1 = READ_ONCE(*p);			\
        barrier();						\
        ___p1;							\
})

/* Page size */
#define PAGE_SHIFT	12
#define PAGE_SIZE	(1 << PAGE_SHIFT)

/* Segment Constants */
#define PRESENT		0x80    /* The segment is present */
#define CODE		0x18    /* A code segment type */
#define DATA		0x10    /* A data segment type */
#define EXP_DOWN	0x04    /* The segment grows downward */
#define CONFORMING	0x04    /* A conforming code segment */
#define READABLE	0x00    /* A readable segment */
#define WRITEABLE	0x02    /* A writeable segment */

/* A standard code segment */
#define CODE_SEG	(CODE | PRESENT | READABLE)
/* A standard data segment */
#define DATA_SEG	(DATA | PRESENT | WRITEABLE)
/* A standard stack segment */
#define STACK_SEG	DATA_SEG
/* A standard TSS segment */
#define TSS_SEG		(PRESENT | 0x09)
/* A real-mode area segment */
#define REAL_SEG	(CODE | PRESENT | READABLE)

/* 32-bit Interrupt Gate */
#define INT_GATE	(PRESENT | 0x0e)
/* 32-bit Trap Gate */
#define TRAP_GATE	(PRESENT | 0x0f)

/* This is the data selector segment for the kernel */
#define KERNEL_DATA		0x08
/* This is the stack selector segment for the kernel */
#define KERNEL_STACK		0x08
/* This is the code selector segment for the kernel */
#define KERNEL_CODE		0x10

#endif /* KERNEL_H */
