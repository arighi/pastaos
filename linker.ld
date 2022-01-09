ENTRY(_start)

SECTIONS {
	.text 0x100000 : AT(0x100000) {
		*(.text*)
		*(.rodata*)
	}
	.data : {
		*(.data*)
	}
	.bss : {
		*(.bss*)
	}
}
