#include "kernel/fcntl.h"
#include "kernel/riscv.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  // Your code here.
  char *mem = sbrk(4096 * 32);
  const char *tag = "This may help.";
  for (int i = 0; i < 32 * 4096; i++) {
    if (strcmp(mem + i, tag) == 0) {
      // printf("%s %p\n", mem + i, mem + i);
      printf("%s\n", mem + i + 16);
    }
  }

  exit(0);
}
