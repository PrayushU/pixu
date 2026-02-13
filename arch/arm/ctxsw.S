.global ctxsw

/* void ctxsw(void **old_sp, void **new_sp) */
ctxsw:
    /* 1. Save current process context */
    push {r4-r11, lr}     @ Save callee-saved registers and return addr
    str  sp, [r0]         @ Save current SP into old_sp (PCB)

    /* 2. Load new process context */
    ldr  sp, [r1]         @ Load new SP from new_sp (PCB)
    pop  {r4-r11, pc}     @ Restore registers and jump to new LR
