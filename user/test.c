#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (match("a*", "hello")) {
    printf("Matched!\n");
  } else {
    printf("Did not match.\n");
  }
  return 0;
}