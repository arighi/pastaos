.section .init

# Multiboot constants
#define MULTIBOOT_PAGE_ALIGN	(1 << 0)
#define MULTIBOOT_MEMORY_INFO	(1 << 1)

#define MULTIBOOT_HEADER_MAGIC	(0x1BADB002)
#define MULTIBOOT_HEADER_FLAGS	(MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)
#define CHECKSUM		-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.global _start
.global boot_info

.extern main
.extern K_STACK_START

_start:
	jmp	entry

	# Multiboot header (required by GRUB)
.align 4
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long CHECKSUM
	# Linear address of a block of system bootstrap informations
.align 4
	boot_info: .long	0
entry:
	# Save multiboot structure pointer.
	mov	%ebx, (boot_info)

	# Reload global descriptor table (GDT)
	lgdtl	(gdtr)

	# Reflush CS register
	ljmp	$0x10, $1f
1:
	# Update segment registers
	movw	$0x08, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	# Initialize kernel stack pointer
	movw	%ax, %ss
	mov	$K_STACK_START, %esp

	# Call init functions
	call init

	# Jump to the kernel main routine
	call main

	# Disable interrupts
	cli
_done:
	hlt
	jmp _done

.align 4
gdtr:
	.word (gdt_end - gdt - 1)	# GDT limit
	.long (gdt)			# GDT linear address

.align 4
gdt:
# Dummy descriptor 0x00
	.word 0			# Limit 15:0
	.word 0			# Base 15:0
	.byte 0			# Base 23:16
	.byte 0			# Access byte (descriptor type)
	.byte 0			# Limits 19:16, Flags
	.byte 0			# Base 31:24

# Data descriptor 0x08
	.word 0xffff		# Limit 15:0
	.word 0			# Base 15:0
	.byte 0			# Base 23:16
	.byte 0x92         	# Data, Present, Writeable (1,0,0,1,0010)
	.byte 0xcf		# G=1, D=1, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,1111)
	.byte 0			# Base 31:24

# Code descriptor 0x10
	.word 0xffff		# Limit 15:0
	.word 0			# Base 15:0
	.byte 0			# Base 23:16
	.byte 0x9a		# Code, Present, Non-conforming, Exec/read (1,0,0,1,1110)
	.byte 0xcf		# G=1, D=1, 0, AVL=0, 1111=F: Limit/Length (1,1,0,0,1111)
	.byte 0			# Base 31:24
gdt_end:
