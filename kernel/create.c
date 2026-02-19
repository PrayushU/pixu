/*  create.c  - create, newpid  */

#include "process.h"
#include "xinu.h"
#include <stdarg.h>

/*-------------------------------------------------------------------
 *  create  - create a process to start running on ARM
 *-------------------------------------------------------------------
 */

pid32 newpid();

#if 0
pid32 create(void *funcaddr, /* address of the function */
             uint32 ssize,   /* stack size in bytes */
             pri16 priority, /* process priority > 0 */
             char *name,     /* process name */
             uint32 nargs,   /* number of args that follow */
             ...             /* arguments for the function */
) {
  uint32 *saddr;         /* stack address */
  pid32 pid;             /* new process id */
  struct procent *prptr; /* pointer to proc. table entry */
  intmask mask;          /* interrupt mask */
  uint32 *pushsp;        /* pointer to walk the stack */
  va_list ap;            /* for variable arguments */
  uint32 i;

  mask = disable();

  /* Round stack size to a multiple of 8 bytes (ARM Alignment) */
  ssize = (uint32)roundmb(ssize);
  if (ssize < MINSTK)
    ssize = MINSTK;

  if (priority < 1 || (pid = newpid()) == SYSERR ||
      ((saddr = (uint32 *)getstk(ssize)) == (uint32 *)SYSERR)) {
    restore(mask);
    return SYSERR;
  }

  prcount++;
  prptr = &proctab[pid];

  /* Initialize process table entry */
  prptr->prstate = PR_SUSP;
  prptr->prprio = priority;
  prptr->prstkbase = (char *)saddr;
  prptr->prstklen = ssize;
  prptr->prparent = (pid32)getpid();
  prptr->prname[PNMLEN - 1] = NULLCH;
  for (i = 0; i < PNMLEN - 1 && (prptr->prname[i] = name[i]) != NULLCH; i++)
    ;

  /* Initialize stack as if it were called: grow downward */
  /* Leave room for the 'pseudo-call' stack frame */
  pushsp = (uint32 *)((uint32)saddr + ssize);
  *--pushsp = (uint32)STACKMAGIC;

  /* Handle Arguments: ARM calling convention (AAPCS) */
  va_start(ap, nargs);

  /* Arguments 5 and above go on the stack */
  /* Note: Simplistic Xinu approach often pushes all args for uniformity,
     but strictly, ARM expects r0-r3 to hold the first 4. */
  uint32 args[10]; // Assume max 10 for safety
  for (i = 0; i < nargs; i++) {
    args[i] = va_arg(ap, uint32);
  }
  va_end(ap);

  /* Push arguments that don't fit in r0-r3 onto the stack */
  for (i = (nargs > 4 ? nargs : 4); i > 4; i--) {
    *--pushsp = args[i - 1];
  }

  /* Set up the context for ctxsw:
     Registers saved: r4-r11, r13(sp), r14(lr), and CPSR */
  *--pushsp = (uint32)funcaddr; /* Program Counter / r15 */
  // *--pushsp = (uint32)INITRET;  /* Link Register / r14 (userret) */
  // *--pushsp = 0;                /* r12 (ip) */
  *--pushsp = 0; /* r11 (v8) */
  *--pushsp = 0; /* r10 (v7) */
  *--pushsp = 0; /* r9  (sb) */
  *--pushsp = 0; /* r8  (v5) */
  *--pushsp = 0; /* r7  (v4) */
  *--pushsp = 0; /* r6  (v3) */
  *--pushsp = 0; /* r5  (v2) */
  *--pushsp = 0; /* r4  (v1) */

  /* Set the initial arguments into the 'saved' r0-r3 slots */
  // *--pushsp = (nargs > 3) ? args[3] : 0; /* r3 */
  // *--pushsp = (nargs > 2) ? args[2] : 0; /* r2 */
  // *--pushsp = (nargs > 1) ? args[1] : 0; /* r1 */
  // *--pushsp = (nargs > 0) ? args[0] : 0; /* r0 */
  //
  /* Push initial CPSR (Supervisor mode, Interrupts enabled) */
  *--pushsp = (uint32)0x00000013;

  *--pushsp = (nargs > 0) ? args[0] : 0;

  prptr->prstkptr = (char *)pushsp;

  restore(mask);
  return pid;
}

#endif

pid32 create(void *funcaddr, uint32 ssize, pri16 priority, char *name,
             uint32 nargs, ...) {
  uint32 *saddr;
  pid32 pid;
  struct procent *prptr;
  intmask mask;
  uint32 *pushsp;
  va_list ap;
  uint32 i;
  uint32 args[4]; // ARM uses R0-R3 for first 4 args

  mask = disable();

  /* 1. Alignment and Allocation */
  ssize = (uint32)roundmb(ssize);
  if (ssize < MINSTK)
    ssize = MINSTK;

  if (priority < 1 || (pid = newpid()) == SYSERR ||
      ((saddr = (uint32 *)getstk(ssize)) == (uint32 *)SYSERR)) {
    restore(mask);
    return SYSERR;
  }

  prcount++;
  prptr = &proctab[pid];

  /* 2. Initialize PCB Metadata */
  prptr->prstate = PR_SUSP;
  prptr->prprio = priority;
  prptr->prstkbase = (char *)saddr;
  prptr->prstklen = ssize;
  prptr->prname[PNMLEN - 1] = NULLCH;
  for (i = 0; i < PNMLEN - 1 && (prptr->prname[i] = name[i]) != NULLCH; i++)
    ;

  /* 3. Initialize the Stack (Grows Downward) */
  pushsp = (uint32 *)((uint32)saddr + ssize);
  pushsp = (uint32 *)((uint32)pushsp & ~0x7);
  // *--pushsp = (uint32)STACKMAGIC;

  /* Gather arguments */
  va_start(ap, nargs);
  for (i = 0; i < 4; i++) {
    args[i] = (i < nargs) ? va_arg(ap, uint32) : 0;
  }
  va_end(ap);

  /* Current Stack Layout at [0x40003058]:
    [0x40003054]: funcaddr (PC)
    [0x40003050]: INITRET (LR)
    [0x40003030]-[0x4000304C]: r4-r11
    [0x4000302C]: CPSR
    [0x40003028]: r0 (Args)
  */

  /* --- THE CONTEXT SWITCH HANDSHAKE --- */

  /* [Word 11] The Program Counter: where we jump to first */
  *--pushsp = (uint32)funcaddr;

  /* [Word 10] The Link Register: where we jump if function returns */
  *--pushsp = (uint32)INITRET; // This should be the address of userret

  /* [Words 9 to 2] Callee-saved registers R11 down to R4 */
  for (i = 0; i < 8; i++) {
    *--pushsp = 0;
  }

  /* [Word 1] The CPSR: SVC Mode (0x13) + Interrupts Enabled (I-bit 0) */
  *--pushsp = (uint32)0x00000013;

  /* [Word 0] The Argument: will be popped into R0 */
  *--pushsp = args[0];

  /* Save this final SP to the PCB so ctxsw knows where to start popping */
  pushsp = (uint32 *)((uint32)pushsp & ~0x7); // the byte alignment
  prptr->prstkptr = (char *)pushsp;

  restore(mask);
  return pid;
}

/*------------------------------------------------------
 *  newpid  -   Obtain a new (free) process ID
 *------------------------------------------------------
 */
pid32 newpid(void) {
  uint32 i;
  static pid32 nextpid = 1;

  /*  Check all NPROC slots */

  for (i = 0; i < NPROC; i++) {
    nextpid %= NPROC; /* Wrap around to beginning */
    if (proctab[nextpid].prstate == PR_FREE) {
      return nextpid++;
    } else {
      nextpid++;
    }
  }
  return (pid32)SYSERR;
}
