#ifndef _TESTS_H_
#define _TESTS_H_

#include <kernel.h>

/* Simple Test Result Macros */
#define TEST_PASS 1
#define TEST_FAIL 0

typedef uint32 shellstat;

/* The Industrial Assertion: Prints file and line on failure */
#define ASSERT(cond, name)                                                     \
  do {                                                                         \
    if (!(cond)) {                                                             \
      kprintf("[FAIL] ");                                                      \
      kprintf(name);                                                           \
      kprintf(" at ");                                                         \
      kprintf(__FILE__);                                                       \
      kprintf(" line: ");                                                      \
      kputn(__LINE__);                                                         \
      kprintf("\n\r");                                                         \
      return TEST_FAIL;                                                        \
    }                                                                          \
  } while (0)

/* Prototypes for your orchestration */
void test_main(void);
shellstat check_kernel_health(void);
shellstat check_process_exhaustion(void);

/* Prototype for specific suites */
status test_create_kill(void);
status test_queues(void);

#endif
