.global ctxsw

/* * void ctxsw(void **old_sp, void **new_sp) 
 * r0 = address of old process's stack pointer field (e.g., &proctab[old].prstkptr)
 * r1 = address of new process's stack pointer field (e.g., &proctab[new].prstkptr)
 */
ctxsw:
    /* 1. SAVE OLD CONTEXT */
    push {r4-r11, lr}       @ Save callee-saved registers and return address
    mrs  r2, cpsr           @ Read current status (mode and interrupt bits)
    push {r2}               @ Save CPSR to stack
    push {r0}               @ Push a placeholder for R0 (keeps stack frame consistent)
    
    str  sp, [r0]           @ Save the current stack pointer into the old PCB

    /* 2. RESTORE NEW CONTEXT */
    ldr  sp, [r1]           @ Switch to the new process's stack pointer

    pop  {r0}               @ Restore R0 (This becomes the 1st argument for the function)
    pop  {r2}               @ Restore saved CPSR into r2
    msr  cpsr_cxsf, r2      @ Update CPU status (Enables interrupts for the new process)

    /* 3. THE FINAL JUMP */
    /* Pop r4-r11 into their registers, 
       Pop the address of userret into LR,
       Pop the address of the function into PC to start execution. */
    pop  {r4-r11, lr, pc}
