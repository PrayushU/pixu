#include "kernel.h"
#include "process.h"
#include "queue.h"
#include "tests.h"
#include "xinu.h"

/* These define the actual memory for your OS structures */
struct procent proctab[NPROC]; /* The Process Table */
struct qentry queuetab[NQENT]; /* The Global Queue Table */
qid16 readylist;               /* Index of the Ready List */
pid32 currpid;                 /* Currently running process ID */

struct defer Defer; /* Reschedule deferral control */

int32 prcount = 1;

uint32 dummy() {
  *((volatile uint32 *)0x09000000) = '!';
  kprintf("ITS IN DUMMY!!!");
  return 0;
}

/* * nulluser - The first C code to run after startup.s
 * For now, we just initialize the system and hang.
 */
void nulluser(void) {

  /*  1. Initialize all queues  */
  for (int32 i = 0; i < NQENT; i++) {
    queuetab[i].qnext = EMPTY;
    queuetab[i].qprev = EMPTY;
  }

  /*  2. Initialize the Ready list  */
  readylist = newqueue();

  kprintf("\n\rWelcome to Xinu on QEMU ARM!\n\r");

  /* 3. Setup the NULL Process Identity */
  currpid = 0;
  struct procent *prptr = &proctab[0];
  prptr->prstate = PR_CURR;
  prptr->prprio = 0;

  char name[] = "NULLPROC";
  prptr->prname[PNMLEN - 1] = NULLCH;
  for (uint32 i = 0; i < PNMLEN - 1 && (prptr->prname[i] = name[i]) != NULLCH;
       i++)
    ;

  kprintf("Xinu Initialized. Null Process running.\n");

  /*  3. Create the first user process  */
  pid32 p1 = create(dummy, 4096, 50, "test_main", 0);

  if (p1 == SYSERR) {
    kprintf("Error in creating process\n\r");
  } else if (p1 > 0) {
    kprintf("Process Created! ");
    kputn(p1);
    kprintf("\n");
    /*  RESUME moves it to ready list and calls RESCHED */
    resume(p1);
    kprintf("Resumed\n\r");
  } else if (p1 == 0) {
    kprintf("Created process is pid 0\n");
  }
  kputn(p1);

  kprintf("\n\rNull process Resumed\n\r");

  while (1)
    ;
}
