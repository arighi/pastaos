#ifndef KERNEL_H
#define KERNEL_H

#define NULL		0

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

/* Segment Constants */
#define PRESENT		0x80    /* The segment is present */
#define CODE		0x18    /* A code segment type */
#define DATA		0x10    /* A data segment type */
#define EXP_DOWN	0x04    /* The segment grows downward */
#define CONFORMING	0x04    /* A conforming code segment */
#define READABLE	0x02    /* A readable segment */
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

/*
 * Initialization routine: memory occupied by this routine can be freed after
 * kernel initialization is completed.
 */
#define __init__	__attribute__((__section__(".init")))

#endif /* KERNEL_H */
