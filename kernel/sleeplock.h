#ifndef _KERNEL_SLEEPLOCK_H_
#define _KERNEL_SLEEPLOCK_H_

#include "types.h"
#include "spinlock.h"

// Long-term locks for processes
struct sleeplock {
  uint locked;       // Is the lock held?
  struct spinlock lk; // spinlock protecting this sleep lock

  // For debugging:
  char *name;        // Name of lock.
  int pid;           // Process holding lock
};

#endif // _KERNEL_SLEEPLOCK_H_