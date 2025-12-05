#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

#define DELIMETER_65 " -\r\t\n./,"

void do_sixfive(char *filename) {
  int fd = open(filename, 0644);
  if (fd < 0) {
    fprintf(2, "sixfive: cannot open %s\n", filename);
    exit(1);
  }

  struct stat st;
  fstat(fd, &st);
  char *buf = malloc(st.size + 1);
  if (buf == 0) {
    fprintf(2, "sixfive: cannot allocate memory\n");
    exit(1);
  }

  int char_left = st.size;
  while (char_left > 0) {
    int count = read(fd, buf + st.size - char_left, char_left);
    char_left -= count;
  }

  char *temp = buf;
  while (*temp) {
    if (strchr(DELIMETER_65, *temp) == 0) {
      temp++;
      continue;
    }
    *temp = '\0';
    temp++;
  }

  char *cur = buf;
  while (cur < temp) {
    if (*cur) {
      int num = atoi(cur);
      if (num % 6 == 0 || num % 5 == 0) {
        fprintf(1, "%d\n", num);
      }
      while (*cur)
        cur++;
    }
    cur++;
  }
  free(buf);
  close(fd);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(2, "Usage: sixfive FILE\n");
    exit(1);
  }

  argv++;
  while (*argv) {
    do_sixfive(*argv);
    argv++;
  }
  exit(0);
}