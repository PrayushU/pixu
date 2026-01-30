/* ready.c - ready*/


#include "xinu.h"


qid16 readylist; /* Index of ready list*/


/*-------------------------------------------------------------
 * ready - Make a process eligible for CPU service
 *--------------------------------------------------------------
*/


status ready(
    pid32 pid;
)
{
    register strcut procent *prptr;

    if(isbadpid(pid)){
        return SYSERR;
    }

    /* Set process state to indicate ready and add to ready list*/

    prptr = &proctabp[pid];
    prptr->prstate = PR_READY;
    insert(pid, readylist, prptr->prprio);
    resched();

    return OK;
}
