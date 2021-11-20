.section .init

# Address adjustement
#define ADDRADJUST	0xBFF00000

# Kernel virtual start address
#define K_VIR_START	0xC0000000

# Kernel physiscal start address
#define K_PHYS_START	0x100000

# Paging constants
#define	P_PRESENT 	0x01
#define P_WRITE		0x02

# Page size
#define PAGE_SHIFT	12
#define PAGE_SIZE	(1 << PAGE_SHIFT)

# Page directory
#define K_PDE		0x1000

# Kernel page table #0 (4MB)
#define K_PTE		0x2000

# First 4MB Identity-map page table
#define I_PTE		0x3000

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
	mov	%ebx, (boot_info - ADDRADJUST)

	# Setup paging
	xorl	%eax, %eax
	mov	$K_PDE, %edi
	mov	$1024, %ecx
	cld
	rep	stosl

	# Setup first 4MB as identity-map (I_PTE)
	mov	$(P_PRESENT | P_WRITE), %eax
	mov	$1024, %ecx
	mov	$I_PTE, %edi
Map_I_PTE:
	stosl
	add	$PAGE_SIZE, %eax
	loop	Map_I_PTE

	# Setup the kernel page table (K_PTE)
	mov	$(K_PHYS_START | P_PRESENT | P_WRITE), %eax
	mov	$1024, %ecx
	mov	$K_PTE, %edi
Map_K_PTE:
	stosl
	add	$PAGE_SIZE, %eax
	loop	Map_K_PTE

	# Store the identity-map table (first 4MB)
	movl $(I_PTE | P_PRESENT | P_WRITE), K_PDE

	# Store the kernel table (4MB of kernel virtual space)
	movl $(K_PTE | P_PRESENT | P_WRITE), K_PDE + (K_VIR_START >> PAGE_SHIFT) / 1024 * 4

	# Map page directory into itself
	movl $(K_PDE | P_PRESENT | P_WRITE), K_PDE + 1023 * 4

	# Setup PDBR
	mov	$K_PDE, %eax
	mov	%eax, %cr3

	# Enable paging
	mov	%cr0, %eax
	orl	$0x80010000, %eax # cr0.PG = cr0.WP = 1
	mov	%eax, %cr0

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

	# Jump to the kernel main routine
	jmp	main

.align 4
gdtr:
	.word (gdt_end - gdt - 1)	# GDT limit
	.long (gdt - ADDRADJUST)	# GDT linear address

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
