#include "process.h"
#include "tests.h"

void test_teardown(void) {
  int32 i;
  for (i = 0; i < NPROC; i++) {
    /* Kill everything except Null process and this test_main process */
    if (i != 0 && i != currpid && proctab[i].prstate != PR_FREE) {
      kill(i);
    }
  }
  /* Critical Detail: Reset any global test counters or flags here */
}

void test_main(void) {
  kprintf("\n\r==============================\n\r");
  kprintf("   XINU INDUSTRIAL TESTER\n\r");
  kprintf("==============================\n\r");

  /* Suite 1 */
  if (test_create_kill() == TEST_PASS) {
    kprintf("RESULT: Create/Kill SUCCESS\n\r");
  } else {
    kprintf("RESULT: Create/Kill FAILED\n\r");
    test_teardown();
  }

  /*
  // /* Suite 2 */
  // if (check_process_exhaustion() == TEST_PASS) {
  //   kprintf("RESULT: Create/Kill SUCCESS\n\r");
  // } else {
  //   kprintf("RESULT: Create/Kill FAILED\n\r");
  //   test_teardown();
  // }
  //
  // /* Run Invariant Check */
  // kprintf("Running Kernel Health Check...\n\r");
  // if (check_kernel_health() == OK) {
  //   kprintf("[HEALTHY] No corruption detected.\n\r");
  // } else {
  //   kprintf("[CRITICAL] Kernel Invariants Violated!\n\r");
  // }
  //
  // kprintf("\n\rAll tests finished. Halting system.\n\r");
  // halt();
}
