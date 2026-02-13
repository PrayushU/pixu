/* getfirst.c - getfirst */

#include "xinu.h"

/*------------------------------------------------------------------------
 * getfirst - Remove a process from the front of a queue and return its PID
 *------------------------------------------------------------------------
 */
pid32 getfirst(qid16 q) {
    pid32 head;

    if (isempty(q)) {
        return EMPTY;
    }

    /* The head of the list is at index q. 
       The first actual process is at queuetab[q].qnext */
    head = queuehead(q);
    return getitem(queuetab[head].qnext);
}