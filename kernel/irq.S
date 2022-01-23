#define ALIGN                   .align 16

.text

# Macro to build a generic interrupt handler routine
# with no error code.
#define EXCEPTION_NOCODE(name, exc)	\
	.global _##name ;		\
	ALIGN ;				\
_##name:				\
	pushl	$0 ;			\
	pushl	##exc ;			\
	jmp	COMMON_EXC ;

# Macro to build a generic interrupt handler routine
# with error code.
#define EXCEPTION(name, exc)		\
	.global _##name ;		\
	ALIGN ;				\
_##name:				\
	pushl	##exc ;			\
	jmp	COMMON_EXC ;		\

# Macro to build a generic interrupt handler routine
#define INTERRUPT(name, irq)		\
	.global _##name ;		\
	ALIGN ;				\
_##name:				\
	pushl	##irq ;			\
	jmp	__common_isr;		\

# Common interrupt procedure.
# Every INTs must call this procedure.
.extern	 default_irq_handler
ALIGN
__common_isr:
	cld
	# Save segment registers.
	push	%ds
	push	%es
	# Save general purpose registers.
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	pushl	%ebx
	pushl	%ebp
	pushl	%esi
	pushl	%edi
	# Call the default handler;
	#     void default_handler(irq_context_t *c);
	pushl	%esp
	call	default_irq_handler
.global ret_from_isr
ret_from_isr:
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
	# Skip the IRQ number.
	add	$4, %esp
	# Return from interrupt; restores %eip, %cs and
	# eflags registers for 0-privileged tasks;
	# for other tasks also %esp and %ss registers
	# are restored.
	iretl

.extern default_exc_handler
ALIGN
COMMON_EXC:
	cld
	# Save segment registers.
	push	%ds
	push	%es
	# Save general purpose registers.
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	pushl	%ebx
	pushl	%ebp
	pushl	%esi
	pushl	%edi
	# Call the default exception handler;
	#     void default_exception(exc_context_t *c);
	pushl	%esp
	call	default_exc_handler
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

### Exception handlers ###

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

### Interrupt handlers ###

INTERRUPT(irq_00, $0x00)		# Timer
INTERRUPT(irq_01, $0x01)		# Keyboard
INTERRUPT(irq_02, $0x02)		# PIC Slave
INTERRUPT(irq_03, $0x03)		# RS232-(COM2) or Available
INTERRUPT(irq_04, $0x04)		# RS232-(COM1)
INTERRUPT(irq_05, $0x05)		# Available
INTERRUPT(irq_06, $0x06)		# Floppy Disk
INTERRUPT(irq_07, $0x07)		# LPT1
INTERRUPT(irq_08, $0x08)		# CMOS Real Time Clock
INTERRUPT(irq_09, $0x09)		# PIC Master
INTERRUPT(irq_0A, $0x0a)		# Available
INTERRUPT(irq_0B, $0x0b)		# Available
INTERRUPT(irq_0C, $0x0c)		# Mouse PS/2 or Available
INTERRUPT(irq_0D, $0x0d)		# Numeric Coprocessor
INTERRUPT(irq_0E, $0x0e)		# IDE Channel 0 (Master)
INTERRUPT(irq_0F, $0x0f)		# IDE Channel 1 (Slave)

INTERRUPT(irq_unhand, $0xff)		# Unhandled interrupt
