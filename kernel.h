/* kernel.h - Master include for Xinu types and constants */


/* Basic integer types with explicit sizes */


typedef int int32;
typedef unsigned int uint32;
typedef short         int16;
typedef unsigned short uint16;
typedef unsigned char byte;
typedef unsigned char bool8;


/* OS specific type names */
typedef int32 pid32;
typedef int32 sid32;
typedef int16 qid16;
typedef int32 did32;
typedef uint32 intmask;
