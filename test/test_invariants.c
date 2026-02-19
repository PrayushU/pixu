#include "kernel.h"
#include <process.h>
#include <queue.h>
#include <tests.h>

extern struct qentry queuetab[];
extern struct procent proctab[];

/**
 * @brief Verifies kernel data structures are not corrupted.
 * @return OK if healthy, SYSERR if corruption detected.
 */
status check_kernel_health(void) {
  int32 i;

  /* 1. Verify Queue Doubly-Linked Integrity */
  for (i = 0; i < NQENT; i++) {
    qid16 next = queuetab[i].qnext;
    qid16 prev = queuetab[i].qprev;

    if (next < NQENT && queuetab[next].qprev != i) {
      kprintf("Invariant Violation: Queue %d next->prev mismatch - ");
      kputn(i);
      kprintf("\n");
      return SYSERR;
    }
    if (prev < NQENT && queuetab[prev].qnext != i) {
      kprintf("Invariant Violation: Queue %d prev->next mismatch - ");
      kputn(i);
      kprintf("\n");
      return SYSERR;
    }
  }

  /* 2. Verify Process Count */
  int32 actual_count = 0;
  for (i = 0; i < NPROC; i++) {
    if (proctab[i].prstate != PR_FREE) {
      actual_count++;
    }
  }
  if (actual_count != prcount) {
    kprintf("Invariant Violation: prcount (%d) != actual (%d) - ");
    kputn(prcount);
    kprintf("-");
    kputn(actual_count);
    kprintf("\n");
    return SYSERR;
  }

  return OK;
}
