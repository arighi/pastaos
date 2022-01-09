.global _start

#define MULTIBOOT_HEADER_MAGIC  (0x1BADB002)
#define MULTIBOOT_HEADER_FLAGS	0
#define MULTIBOOT_CHECKSUM	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

_start:
	jmp entry
.align 4
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long MULTIBOOT_CHECKSUM
	.long 0
.align 4

entry:
	pushl $0xdeadbeef
	pushl $0x01
	call main

	# dead end
	cli
	hlt
	jmp entry
