/*  stack.c - getstk() , freestk () */

#include "xinu.h"

/*-----------------------------------------------------------------
 *  getstk - Allocates memory for stack and returns base address
 *-----------------------------------------------------------------
 */
char *getstk(uint32 size) { return (void *)0; }

/*--------------------------------------------------------------------------
 *  freestk -  Free all allocated memory for from base stack to stack length
 *--------------------------------------------------------------------------
 */
void freestk(char *stkbase, uint32 stklen) {
  while (--stklen) {
  }
}
