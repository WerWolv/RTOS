LOAD_ADDR = 0x00000;

ENTRY(entry)

MEMORY {
	RAM     (rwx)   :   ORIGIN = 0x20000000, LENGTH = 128K
    SRAM    (rw )   :   ORIGIN = 0x20020000, LENGTH = 384K
    FLASH   (rx )   :   ORIGIN = 0x08000000, LENGTH = 1024K    
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {

	. = LOAD_ADDR;
	__stack_top = LOAD_ADDR;

    .vtor : {
        . = ALIGN(4);
        KEEP(*(.vtor))
        . = ALIGN(4);
    } > FLASH

	.text : { 
		. = ALIGN(4);

        *(.text)
        *(.text.*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;
	} > FLASH

	.rodata : {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
	} > FLASH

    .ARM.extab   : {
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    .ARM : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } > FLASH

    .preinit_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } > FLASH

    .init_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } > FLASH

    .fini_array : {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } > FLASH

	.bss (NOLOAD) : {
		. = ALIGN(16);
        _sbss = .;
		__bss_start = _sbss;
		*(.bss .bss.*)
        *(COMMON)
		. = ALIGN(8);
        _ebss = .;
        __bss_end = _ebss;
	} > RAM
	__bss_size = (__bss_end - __bss_start);
	
    _sidata = LOADADDR(.data);

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        *(.RamFunc)
        *(.RamFunc*)
        *(.gb_rom)
        . = ALIGN(4);
        _edata = .;

    } > RAM AT > FLASH

	/DISCARD/ : {
		*(.interp)
		*(.dynsym)
		*(.gnu.hash)
		*(.hash)
		*(.dynstr)
		*(.plt)
		*(.dynamic)
		*(.rela*)

        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
	}

    .ARM.attributes 0 : { *(.ARM.attributes) }

}