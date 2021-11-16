# Set magic number to 0x1BADB002 to identified by the boot loader (e.g., GRUB)
.set MAGIC,    0x1BADB002

# Set flags to 0
.set FLAGS,    0

# Set the checksum
.set CHECKSUM, -(MAGIC + FLAGS)

# Set multiboot enabled
.section .multiboot

# Define each data declared above
.long MAGIC
.long FLAGS
.long CHECKSUM

# Define the stack
stack_bottom:
.skip 1024
stack_top:

# Section text is where the code starts (boot loader jumps here)
.section .text
.global _start
.type _start, @function

_start:
	# Assign current stack pointer to stack_top
	mov $stack_top, %esp

	# Call the kernel main routine
	call main

	# Disable interrupts
	cli

_done:
	hlt
	jmp _done

.size _start, . - _start
