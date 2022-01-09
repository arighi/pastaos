#ifndef IO_H
#define IO_H

#include <types.h>

/* Low-level hardware port I/O */

static inline void outb(uint8_t val, uint16_t port)
{
	asm volatile("outb %0, %1" : : "a" (val), "dN" (port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t val;

	asm volatile("inb %1, %0" : "=a" (val) : "dN" (port));

	return val;
}

#endif /* IO_H */
