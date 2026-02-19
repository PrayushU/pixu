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

  prcount--; /*The prcistence*/

  /* the following is not correct as we like 'Presistence' state of OS in for
   * bare-metal OS. It stays running and would wait for new command or for
   * network packet to arrive. */
  // if (prcount <= 1) {
  //   xdone();
  // }
  freestk(prptr->prstkbase, prptr->prstklen);

  switch (prptr->prstate) {
  case PR_CURR:
    prptr->prstate = PR_FREE;
    resched();
    break;

  case PR_READY:
    getitem(pid);
    prptr->prstate = PR_FREE;
    break;
  case PR_SLEEP:
  case PR_WAIT:
    getitem(pid);
    prptr->prstate = PR_FREE;
    break;

  default:
    prptr->prstate = PR_FREE;
  }

  restore(mask);
  return OK;
}
