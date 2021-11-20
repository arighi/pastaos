K_VIR_START = 0xC0000000;
K_PHYS_START = 0x100000;
PAGE_SIZE = 0x1000;
K_STACK_SIZE = PAGE_SIZE;

ENTRY (_start)

SECTIONS
{
	.text K_VIR_START : AT(K_PHYS_START)
	{
		KERNEL_INIT = .;
		*(.init*)
		. = ALIGN(PAGE_SIZE);
		KERNEL_END_INIT = .;

		KERNEL_TEXT = .;
		*(.text*)
		*(.rodata*)
		. = ALIGN(PAGE_SIZE);
		KERNEL_END_TEXT = .;
	}

	.data : AT(K_PHYS_START + (KERNEL_DATA - KERNEL_INIT))
	{
		KERNEL_DATA = .;
		*(.data*)
		. = ALIGN(PAGE_SIZE);
		KERNEL_END_DATA = .;
	}

	.bss : AT(K_PHYS_START + (KERNEL_BSS - KERNEL_INIT))
	{
		_stack_end = .;
		. += K_STACK_SIZE;
		_stack_start = .;
		K_STACK_START = .;

		. = ALIGN( PAGE_SIZE );
		KERNEL_BSS = .;
		*(.bss*)
		*(COMMON*)
		. = ALIGN(PAGE_SIZE);
		KERNEL_END_BSS = .;
	}
	. = ALIGN(PAGE_SIZE);
	KERNEL_TOP = .;
}
