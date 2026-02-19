/*  userret.c - userret */

#include "xinu.h"

/*---------------------------------------------------------------------
 *  userret - Called when a process return from the top-level function
 *---------------------------------------------------------------------
 */
void userret(void) {
  kprintf("\nIn userret!\n");
  kill(getpid());
}
