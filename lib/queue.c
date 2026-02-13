/* queue.c - enqueue, dequeue*/
#include "xinu.h"


pid32 enqueue(
    pid32 pid, /*ID of process to insert*/
    qid16 q)    /*Id of queue to use*/
{

    qid16   tail, prev; /* Tail & previous node indexes*/

    if(isbadqid(q) || isbadpid(pid)){
        return SYSERR;
    }

    tail = queuetail(q);
    prev = queuetab[tail].qprev;


    queuetab[pid].qnext = tail; /* Insert just before tail node*/
    queuetab[pid].qprev = prev;
    queuetab[prev].qnext = pid;
    queuetab[tail].qprev = pid;
    return pid;
}




pid32 dequeue(
    qid16 q
)
{
    pid32 pid;
    if(isbadqid(q)){
        return SYSERR;
    } else if(isempty(q)){
        return EMPTY;
    }


    pid = getfirst(q);
    queuetab[pid].qprev = EMPTY;
    queuetab[pid].qnext = EMPTY;
    return pid;
}