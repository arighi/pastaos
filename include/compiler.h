#ifndef COMPILER_H
#define COMPILER_H

/*
 * 'fallthrough' keyword for case statements blocks
 * (see https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes)
 */
#define fallthrough	__attribute__((__fallthrough__))

/*
 * Initialization routine: memory occupied by this routine can be freed after
 * kernel initialization is completed.
 */
#define __init__	__attribute__((__section__(".init")))

#endif /* COMPILER_H */
