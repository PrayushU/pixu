/* queue.h - firstid, firstkey, isempty, lastkey, noempty     */

/* Queue structure declarations, constants, and inline functions  */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "kernel.h"

/*Default # of queue entries: 1 per process plus 2 for ready list plus  */
/*      2 for sleep list plus 2 per semaphore                           */

#ifndef NQENT
#define NQENT (NPROC + 4 + NSEM + NSEM)
#endif

#define EMPTY (-1) /* Nill value for qnext or qprev index */
#define MAXKEY 0x7FFFFFFF
#define MINKEY 0x80000000

struct qentry
{              /* One per process plus two per list  */
  int32 qkey;  /* Key to which the queue is ordered  */
  qid16 qnext; /* Index of next process or tail      */
  qid16 qprev; /* Index of previous process or head  */
};

extern struct qentry queuetab[];
extern qid16 readylist;

/* Inline queue manipulation functions  */

#define queuehead(q) (q)
#define queuetail(q) ((q) + 1)
#define firstid(q) (queuetab[queuehead(q)].qnext)
#define lastid(q) (queuetab[queuetail(q)].qprev)
#define isempty(q) (firstid(q) >= NPROC)
#define nonempty(q) (firstid(q) < NPROC)
#define firstkey(q) (queuetab[firstid(q)].qkey)
#define lastkey(q) (queuetab[lastid(q)].qkey)

/* Inline to check queue id assumes interrupts are disabled */

#define isbadqid(x) (((int32)(x) < 0) || (int32)(x) >= NQENT - 1)

/* Function Prototypes for Queue Manipulation */
pid32   getfirst(qid16 q);
pid32   getlast(qid16 q);
pid32   getitem(pid32 pid);
pid32   enqueue(pid32 pid, qid16 q);
pid32   dequeue(qid16 q);
status  insert(pid32 pid, qid16 q, int32 key);
qid16   newqueue(void);

#endif
