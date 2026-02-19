/*  porcess.h - isbadpid */
#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "kernel.h"

/* Maximum number of process in the system*/

#ifndef NPROC
#define NPROC 8
#endif

/* Process state constants*/

#define PR_FREE 0
#define PR_CURR 1
#define PR_READY 2
#define PR_RECV 3
#define PR_SLEEP 4
#define PR_SUSP 5
#define PR_WAIT 6
#define PR_RECTIM 7

#define PNMLEN 16 /* Length of proces "name"*/
#define NDESC 5   /*number of descriptors a process can have open*/

/* Process initialisation contants*/

#define INITSTK 65536   /* Initial process stack size*/
#define INITPRIO 20     /* Initial process priorityh*/
#define INITRET userret /* Address to which process returns*/

/* Inline code to check process ID (assumes interrupts are disabled)*/

#define isbadpid(x)                                                            \
  (((pid32)(x) < 0) || ((pid32)(x) >= NPROC) || (proctab[x].prstate == PR_FREE))

struct procent {
  uint16 prstate;
  pri16 prprio;
  char *prstkptr;      /*saved stack pointer*/
  char *prstkbase;     /* Base of run time stack*/
  uint32 prstklen;     /*stack length in bytes*/
  char prname[PNMLEN]; /*Process name*/
  sid32 prsem;         /*semaphore on which process waits*/
  pid32 prparent;      /* Id of the creating process*/
  // umsg32  prmsg; /*Message sent to this process*/
  bool8 prhasmsg;      /*Nonzero if msg is valid*/
  int16 prdesc[NDESC]; /*Device descriptor for process*/
};

/* Marker for the top of a process stack (used to hep detect overflow)*/
#define STACKMAGIC 0x0A0AAAA9

extern struct procent proctab[];
extern int32 prcount; /* Currently active processes*/
extern pid32 currpid; /*Currently executing process*/

/* Deferring constants */
#define DEFER_START 1
#define DEFER_STOP 2

/* Structure to track deferral status */
struct defer {
  int32 ndefers; /* Number of active defers */
  bool8 attempt; /* Was resched called during defer? */
};

extern struct defer Defer;

pid32 getpid();
syscall getprio(pid32 pid);

/* Function Prototypes for Process Management */
status
ready(pid32 pid); /* Change process state to PR_READY and puts in ready queue */
void resched(void); /*  Runs highest prioirty process from ready list */
void ctxsw(void *old_sp, void *new_sp); /* Defined in ctxsw.S */
pri16 resume(pid32 pid);                /* Resumes process  */

syscall kill(pid32 pid);
void xdone(void); /* For when process has reached the last process */
void halt(void);  /* Halts process in infinite while loop */

void userret(void);

pid32 newpid();

pid32 create(void *funcaddr, uint32 ssize, pri16 priority, char *name,
             uint32 nargs, ...);

pri16 chprio(pid32 pid, pri16 newprio);

#endif
