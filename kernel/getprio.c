/*  getprio.c - getprio */

#include "kernel.h"
#include "queue.h"
#include "xinu.h"

/*-------------------------------------------------------
 *  getprio - Return the scheduling priority of a process
 *-------------------------------------------------------
 */
syscall getprio(pid32 pid) {
  intmask mask;
  struct procent *prptr;

  if (isbadqid(pid)) {
    restore(mask);
    return SYSERR;
  }

  prptr = &proctab[pid];
  restore(mask);
  return prptr->prprio;
}
