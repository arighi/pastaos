#include <kernel.h>

void __attribute__ ((__noinline__))
__switch_to(uint32_t *new_stack, uint32_t **old_stack)
{
	__asm__ __volatile__(
		     "pushl %%eax\n"
		     "pushl %%ecx\n"
		     "pushl %%edx\n"
		     "pushl %%ebx\n"
		     "pushl %%ebp\n"
		     "pushl %%esi\n"
		     "pushl %%edi\n"
		     "mov %%esp, (%1)\n"
		     "mov %0, %%esp\n"
		     "popl %%edi\n"
		     "popl %%esi\n"
		     "popl %%ebp\n"
		     "popl %%ebx\n"
		     "popl %%edx\n"
		     "popl %%ecx\n"
		     "popl %%eax\n"
		     "ret" : "=r"(new_stack), "=r"(old_stack) :
		              "r"(new_stack), "r"(old_stack) : "memory");
}
