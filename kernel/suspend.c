/* suspend.c - suspend */

#include "process.h"
#include "xinu.h"

/*---------------------------------------------------------------
 *  suspend - suspend a process, placing it in hibernation
 *---------------------------------------------------------------
 */
syscall suspend(pid32 pid) {
  intmask mask;
  struct procent *prptr;
  pri16 prio;

  mask = disable();
  if (isbadpid(pid) || pid == NULLPROC) {
    restore(mask);
    return SYSERR;
  }

  /* Only suspecd a process that is current or ready */

  prptr = &proctab[pid];
  if ((prptr->prstate != PR_CURR) && (prptr->prstate != PR_READY)) {
    restore(mask);
    return SYSERR;
  }

  if (prptr->prstate == PR_READY) {
    getitem(pid); // removes item from its list

    prptr->prstate = PR_SUSP;
  } else {
    prptr->prstate = PR_SUSP;
    resched(); /* suspend and resched */
  }

  prio = prptr->prprio;
  restore(mask);
  return prio;
}
