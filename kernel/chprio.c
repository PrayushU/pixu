/*  chprio.c  - chprio  */

#include "kernel.h"
#include "process.h"
#include "xinu.h"

/*-----------------------------------------------------
 *  chprio  -   Change process priority
 *  @return - returns old process priority
 *-----------------------------------------------------
 */

pri16 chprio(pid32 pid, pri16 newprio) {

  intmask mask;
  struct procent *prptr;
  pri16 oldprio;

  mask = disable();
  if (isbadpid(pid) || newprio < 1) {
    restore(mask);
    return SYSERR;
  }

  prptr = &proctab[pid];
  oldprio = prptr->prprio;

  if (prptr->prstate == PR_READY) {
    getitem(pid);
    prptr->prprio = newprio;
    ready(pid);
  } else {
    prptr->prprio = newprio;
  }

  resched();
  restore(mask);
  return oldprio;
}
