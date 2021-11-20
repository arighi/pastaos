#define KERNEL_DATA_SEL         0x08
#define ALIGN                   .align 16

# Macro to build a generic interrupt handler routine
# with no error code.
#define EXCEPTION_NOCODE(name, exc) \
	.global _##name ; \
	ALIGN ; \
_##name: \
	pushl	$0 ; \
	pushl	##exc ; \
	jmp	COMMON_EXC ;

# Macro to build a generic interrupt handler routine
# with error code.
#define EXCEPTION(name, exc) \
	.global _##name ; \
	ALIGN ; \
_##name: \
	pushl	##exc ; \
	jmp	COMMON_EXC ; \

.extern default_exception
ALIGN
COMMON_EXC:
	cld
	# Saving segment registers; get rid of
	# %gs and %fs registers because the kernel
	# does not use them.
	push	%ds
	push	%es
	# Saving general purpose registers.
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	pushl	%ebx
	pushl	%ebp
	pushl	%esi
	pushl	%edi
	# Update segment registers with the kernel
	# data segment.
	movw	$KERNEL_DATA_SEL, %bx
	movw	%bx, %ds
	movw	%bx, %es
	# Call the default exception handler;
	#     void default_exception(exc_context_t *c);
	pushl	%esp
	call	default_exception
.global ret_from_exc
ret_from_exc:
	popl	%esp
	# Restore general purpose registers.
	popl	%edi
	popl	%esi
	popl	%ebp
	popl	%ebx
	popl	%edx
	popl	%ecx
	popl	%eax
	# Restore segment registers.
	pop	%es
	pop	%ds
	# Skip the exception and error numbers.
	add	$8, %esp
	# Return from interrupt; restores %eip, %cs and
	# eflags registers for 0-privileged tasks;
	# for other tasks also %esp and %ss registers
	# are restored.
	iretl

EXCEPTION_NOCODE(exc_00, $0x00)		# Division by Zero
EXCEPTION_NOCODE(exc_01, $0x01)		# Debug Trap
EXCEPTION_NOCODE(exc_02, $0x02)		# NMI Interrupt
EXCEPTION_NOCODE(exc_03, $0x03)		# Breakpoint Exception
EXCEPTION_NOCODE(exc_04, $0x04)		# Overflow
EXCEPTION_NOCODE(exc_05, $0x05)		# Bound Range Exceeded
EXCEPTION_NOCODE(exc_06, $0x06)		# Invalid Opcode
EXCEPTION_NOCODE(exc_07, $0x07)		# No Math
EXCEPTION(exc_08, $0x08)		# Double Fault
EXCEPTION_NOCODE(exc_09, $0x09)		# 80x87 Overflow
EXCEPTION(exc_0A, $0x0A)		# Invalid TSS
EXCEPTION(exc_0B, $0x0B)		# Segment Not Present
EXCEPTION(exc_0C, $0x0C)		# Stack Fault
EXCEPTION(exc_0D, $0x0D)		# General Protection Fault
EXCEPTION(exc_0E, $0x0E)		# Page Fault
EXCEPTION_NOCODE(exc_0F, $0x0F)		# Intel Reserved

EXCEPTION_NOCODE(exc_unhand, $0x09)	# Unhandled exception
