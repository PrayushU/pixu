/* resched.c - resched, resched_cntl */

#include "xinu.h"

/*--------------------------------------------------------------------
 * resched - Reschedule processor to highest priority eligible process
 *--------------------------------------------------------------------
 */

void resched(void) {
  struct procent *ptold; /* Ptr to table entry for old process*/
  struct procent *ptnew; /* Ptr to table entry for new process*/

  /* Point to process table entry for the current (old) process */
  ptold = &proctab[currpid];

  /* If rescheduling is deferrred, record attmpt and return*/
  if (Defer.ndefers > 0) {
    Defer.attempt = TRUE;
    return;
  }

  // Add this debug print:
  kprintf("\nResched: Old PID ");
  kputn(currpid);
  kprintf(" SP: ");
  kputn((uint32)ptold->prstkptr);

  if (ptold->prstate == PR_CURR) { /* Process remains eligible */
    if (ptold->prprio >= firstkey(readylist)) {
      return;
    }

    /* Old process process will no longer remain current*/
    ptold->prstate = PR_READY;
    insert(currpid, readylist, ptold->prprio);
  }

  /* Force context switch to highest prioirty ready process*/

  currpid = dequeue(readylist);
  ptnew = &proctab[currpid];
  ptnew->prstate = PR_CURR;

  // Add this debug print:
  kprintf(" -> New PID ");
  kputn(currpid);
  kprintf(" SP: ");
  kputn((uint32)ptnew->prstkptr);
  kprintf("\n");

  // preempt = QUANTUM; /* Reset time slice for process*/
  ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

  /* Old process return here when resumed */
  return;
}

/*----------------------------------------------
 * resched_cntl - Control wether rescheduling is deferred or allowed
 *-----------------------------------------------
 */
status resched_cntl(int32 defer) {
  switch (defer) {
  case DEFER_START: /* Handler a deferral request */
    if (Defer.ndefers++ == 0) {
      Defer.attempt = FALSE;
    }
    return OK;
  case DEFER_STOP: /* Hander end of deferral */
    if (Defer.ndefers <= 0) {
      return SYSERR;
    }
    if ((--Defer.ndefers == 0) && Defer.attempt) {
      resched();
    }
    return OK;
  default:
    return SYSERR;
  }
}
