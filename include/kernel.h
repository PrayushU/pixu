/* kernel.h - Master include for Xinu types and constants */

#ifndef _KERNEL_
#define _KERNEL_

#define SYSERR (-1)
#define OK (1)

#define TRUE 1
#define FALSE 0

#define NSEM 100

#define NULLPROC 0

#define NULLCH '\0'

/* Basic integer types with explicit sizes */

typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned char byte;
typedef unsigned char bool8;

/* OS specific type names */
typedef int32 pid32;
typedef int32 sid32;
typedef int16 qid16;
typedef int32 did32;
typedef uint32 intmask;
typedef uint32 status;
typedef uint16 pri16;
typedef uint32 syscall;

void kputc(char c);
void kprintf(char *fmt);
void kputn(uint32 val);

#define roundmb(x) (char *)((7 + (uint32)(x)) & (~7))

#endif
