/*  stack.c - getstk() , freestk () */

#include "xinu.h"

/*  Temporary static block to act as memory for stacks  */
static char fake_heap[65536];
static char *next_stk = fake_heap;

/*-----------------------------------------------------------------
 *  getstk - Allocates memory for stack and returns base address
 *-----------------------------------------------------------------
 */
char *getstk(uint32 size) {
  char *stack_base = next_stk;
  next_stk += size;

  /*  Ensure the next allocation is 8-bytes aligned */
  next_stk = (char *)(((uint32)next_stk + 7) & ~7);
  return stack_base;
}

/*--------------------------------------------------------------------------
 *  freestk -  Free all allocated memory for from base stack to stack length
 *--------------------------------------------------------------------------
 */
void freestk(char *stkbase, uint32 stklen) {
  while (--stklen) {
  }
}
