/* kill.c - kill */

#include "kernel.h"
#include "process.h"
#include "queue.h"
#include "xinu.h"

/*----------------------------------------------------------------------------
 *  kill  - Kill a process and remove it from the system
 *----------------------------------------------------------------------------
 */

syscall kill(pid32 pid) {
  intmask mask;
  struct procent *prptr;
  int32 i;

  mask = disable();
  if (isbadpid(pid) || pid == NULLPROC ||
      (prptr = &proctab[pid])->prstate == PR_FREE) {
    restore(mask);
    return SYSERR;
  }

  if (--prcount <= 1) {
    xdone();
  }

  freestk(prptr->prstkbase, prptr->prstklen);

  switch (prptr->prstate) {
  case PR_CURR:
    prptr->prstate = PR_FREE;
    resched();
  case PR_SLEEP:
  case PR_RECTIM:
  case PR_WAIT:
  case PR_READY:
    getitem(pid);
  default:
    prptr->prstate = PR_FREE;
  }

  restore(mask);
  return OK;
}
