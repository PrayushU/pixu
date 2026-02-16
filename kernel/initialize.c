#include "xinu.h"

/* These define the actual memory for your OS structures */
struct procent proctab[NPROC]; /* The Process Table */
struct qentry queuetab[NQENT]; /* The Global Queue Table */
qid16 readylist;               /* Index of the Ready List */
pid32 currpid;                 /* Currently running process ID */

struct defer Defer; /* Reschedule deferral control */

int32 prcount = 1;

/* * nulluser - The first C code to run after startup.s
 * For now, we just initialize the system and hang.
 */
void nulluser(void) {

  /* 1. Initialize the Global Queue Table */
  /* This sets up the head/tail pointers for all lists */

  /* 2. Initialize the Ready List */
  readylist = newqueue();
  /* 3. Initialize the Process Table */
  /* (You will fill this in as you reach Chapter 5) */

  /* For now, just prove we are alive */
  kprintf("\n\rWelcome to Xinu on QEMU ARM!\n\r");
  kprintf("Milestone 1-3 complete. Queues initialized.\n\r");
  while (1)
    ;
}
