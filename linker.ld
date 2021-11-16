/* Kernel entry point */
ENTRY(_start)

SECTIONS
{
	/* Reserve 1MB of space */
	. = 1M;

	/* Text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data section */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Data section */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* BSS section */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
