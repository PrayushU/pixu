/*  create.c  - create, newpid  */

#include "kernel.h"
#include "process.h"
#include "xinu.h"

/*-------------------------------------------------------------------
 *  create  - create a process to start running on ARM
 *-------------------------------------------------------------------
 */

pid32 newpid();

pid32 create(void *funcaddr, /* address of the function */
             uint32 ssize,   /*  stack size  */
             pri16 priority, /* process priority > 0 */
             char *name,     /* process name */
             uint32 nargs    /*  Number of args that follow */
) {

  uint32 savsp, *pushsp;
  pid32 pid;
  struct procent *prptr;
  intmask mask;
  uint32 *saddr;
  int32 i;

  mask = disable();
  if (ssize < MINSTK) {
    ssize = MINSTK;
  }

  if ((priority < 1) || ((pid = newpid()) == SYSERR) ||
      ((saddr = (uint32 *)getstk(ssize)) == (uint32 *)SYSERR)) {
    restore(mask);
    return SYSERR;
  }

  prcount++;
  prptr = &proctab[pid];

  /*  Initialise the process table entry for new process  */
  prptr->prstate = PR_SUSP;
  prptr->prprio = priority;
  prptr->prstkbase = (char *)saddr;
  prptr->prstklen = ssize;
  prptr->prname[PNMLEN - 1] = NULLCH;
  for (i = 0; i < PNMLEN - 1 && (prptr->prname[i] = name[i]) != NULLCH; i++)
    ;

  prptr->prparent = (pid32)getpid();

  /*  Initialise stack as if it was called */
  *saddr = STACKMAGIC;
  savsp = (uint32)saddr;

  /*  push arguments  */

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
