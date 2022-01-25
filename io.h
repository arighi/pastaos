#ifndef IO_H
#define IO_H

#include <types.h>

static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %%al, %%dx" : : "d"(port), "a"(val));
}

#endif /* IO_H */
