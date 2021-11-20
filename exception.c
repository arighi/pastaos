#include "kernel.h"

typedef struct exception_context
{
	/* General purpose register */
	uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
	/* Segment register */
	uint32_t es, ds;
	/* The exception number and error code */
	uint32_t EXC, err_code;
	/* Instruction pointer */
	uint32_t eip;
	/* Code segment register */
	uint32_t cs;
	/* EFLAGS register */
	uint32_t        eflags;
} __attribute__ ((packed)) exception_context_t;

void default_exception(exception_context_t __attribute__((__unused__)) *c)
{
}
