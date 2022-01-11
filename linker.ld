ENTRY(_start)

PAGE_SIZE = 0x1000;
KERNEL_STACK_SIZE = PAGE_SIZE;

SECTIONS {
	.text 0x100000 : AT(0x100000) {
		*(.text*)
		*(.rodata*)
	}
	.data : {
		. = ALIGN(PAGE_SIZE);
		*(.data*)
	}
	.bss : {
		. = ALIGN(PAGE_SIZE);
		KERNEL_STACK_END = .;
		. += KERNEL_STACK_SIZE;
		KERNEL_STACK_BEGIN = .;

		. = ALIGN(PAGE_SIZE);
		*(.bss*)
	}
}
