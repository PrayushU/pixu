/* linker.ld - Linker script for ARM QEMU (virt machine) */

ENTRY(_start)

SECTIONS
{
    /* QEMU 'virt' loads the kernel at 0x40000000 */
    . = 0x40000000;

    .text : {
        /* Ensure startup code is FIRST */
        _text_start = .;
        KEEP(*(.text.boot)) /* Force the startup code to the front */
        *(.text)
        *(.text.*)
        _text_end = .;
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    /* Reserve 64KB for the inisial system stack */
    . = ALIGN(8);
    . = . + 0x4000;
    _stack_top = .;

    /* Define the end of the OS image for the Memory Manager */
    _end = .;
}
