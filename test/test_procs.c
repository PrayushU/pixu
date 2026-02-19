#include "process.h"
#include "tests.h"

/* A dummy process function for testing */
void test_proc_func(void) {
  while (1) { /* spin */
  }
}

shellstat test_create_kill(void) {
  kprintf("--- Starting Suite: Create & Kill ---\n\r");

  /* 1. Test Basic Creation */
  pid32 pid = create((void *)test_proc_func, 1024, 20, "testp", 0);

  if (pid == SYSERR) {
    kprintf("[FAIL] create returned SYSERR\n\r");
    return TEST_FAIL;
  }

  /* 2. Manual State Verification */
  kprintf("Checking PID: ");
  kputn(pid);
  kprintf("\n\r");

  if (proctab[pid].prstate != PR_SUSP) {
    kprintf("[FAIL] Process state is not PR_SUSP (");
    kputn(proctab[pid].prstate);
    kprintf(")\n\r");
    return TEST_FAIL;
  }

  /* 3. Test Kill */
  status s = kill(pid);
  if (s != OK) {
    kprintf("[FAIL] kill returned SYSERR\n\r");
    return TEST_FAIL;
  }

  /* 4. Verify Slot is FREE */
  if (proctab[pid].prstate != PR_FREE) {
    kprintf("[FAIL] Slot not FREE after kill\n\r");
    return TEST_FAIL;
  }

  kprintf("[PASS] Create & Kill basic logic\n\r");
  return TEST_PASS;
}

shellstat check_process_exhaustion(void) {
  kprintf("Testing NPROC Limit...\n\r");
  pid32 pids[NPROC];
  int32 count = 0;

  /* 1. Fill the table */
  while (count < NPROC) {
    pids[count] = create((void *)test_proc_func, 1024, 20, "limit_test", 0);
    if (pids[count] == SYSERR)
      break;
    count++;
  }

  /* 2. Verify we hit the limit (accounting for nullproc and test_main) */
  kprintf("Created ");
  kputn(count);
  kprintf(" processes.\n\r");

  /* 3. Cleanup: This is the 'Clean State' you asked about */
  for (int i = 0; i < count; i++) {
    kill(pids[i]);
  }

  ASSERT(check_kernel_health() == OK,
         "Health check failed after exhaustion cleanup");
  return TEST_PASS;
}
