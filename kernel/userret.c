/*  userret.c - userret */

#include "xinu.h"

/*---------------------------------------------------------------------
 *  userret - Called when a process return from the top-level function
 *---------------------------------------------------------------------
 */
void userret(void) { kill(getpid()); }
