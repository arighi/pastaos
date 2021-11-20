#include <kernel.h>

typedef struct irq_context {
	/* General purpose register */
	uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
	/* Segment register */
	uint32_t es, ds;
	/* The interrupt number */
	uint32_t IRQ;
	/* Instruction pointer */
	uint32_t eip;
	/* Code segment register */
	uint32_t cs;
	/* EFLAGS register */
	uint32_t eflags;
} __attribute__ ((packed)) irq_context_t;

void default_handler(irq_context_t __attribute__((__unused__)) *c)
{
}
