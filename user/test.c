#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char *argv_exec[] = {"echo", "hi","wc", 0};
  int fd = fork();
  if (fd == 0) {
    exec("echo", argv_exec);
  }
  wait(0);
  return 0;
}