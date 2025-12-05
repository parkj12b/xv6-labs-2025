#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int uptime_ticks = uptime();
  printf("Uptime: %d ticks\n", uptime_ticks);
  return 0;
}