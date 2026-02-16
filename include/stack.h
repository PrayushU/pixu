/*  stack.h */

#ifndef STACK
#define STACK

#include "kernel.h"

#define MINSTK 1024

char *getstk(uint32 size);
void freestk(char *prstkbase, uint32 prstklen);

#endif // STACK
