/* startup.s - Entry point for QEMU ARM Virt */

.section .text.boot, "ax"
.global _start

_start:
    /* 1. Set up the Stack Pointer using the linker symbol */
    ldr sp, =_stack_top

    /* AAPCS Requirement: Ensure 8-byte alignment */
    bic sp, sp, #7

    /* 2. Clear the BSS section */
    /* This ensures all uninitialized global variables are 0 */
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
bss_loop:
    cmp r0, r1
    bge bss_done
    str r2, [r0], #4        /* Store 0 and increment r0 by 4 */
    b bss_loop

bss_done:
    /* 3. Jump to the Xinu Core */
    /* nulluser is the 'main' entry point of the Xinu heart */
    bl nulluser

    /* If nulluser ever returns, just hang in a loop */
halt:
    wfe
    b halt
