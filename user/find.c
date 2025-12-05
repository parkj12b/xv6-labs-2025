#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int exec_flag = 0;
int last_arg_index = 0;
char *exec_cmd = "";
char **exec_args = 0;

void search_and_print(char *path, char *filename) {
  int fd = open(path, O_RDONLY);

  if (fd < 0) {
    return;
  }

  //   fprintf(2, "fd: %d\n", fd);
  struct stat st;
  struct dirent de;

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  char buf[512];
  strcpy(buf, path);
  int path_len = strlen(buf);
  if (buf[path_len - 1] != '/') {
    buf[path_len] = '/';
    buf[path_len + 1] = '\0';
    path_len++;
  }

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;

    // Skip "." and ".." entries
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    // fprintf(2, "de.name: %s\n", de.name);
    // fprintf(2, "buf before: %s\n", buf);
    // Construct new path
    strcpy(buf + path_len, de.name);
    buf[path_len + strlen(de.name)] = '\0';

    struct stat entry_st;
    int entry_fd = open(buf, O_RDONLY);
    if (entry_fd < 0) {
      fprintf(2, "find: cannot open %s\n", buf);
      continue;
    }
    if (fstat(entry_fd, &entry_st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      close(entry_fd);
      continue;
    }
    close(entry_fd);

    if (entry_st.type == T_DIR) {
      // Recurse into directory
      search_and_print(buf, filename);
    } else if (entry_st.type == T_FILE) {
      // Check if filename matches (with regex support)
      if (match(filename, de.name)) {
        if (!exec_flag)
          printf("%s\n", buf);
        else {
          if (fork() == 0) {
			int fd = open(buf, O_RDONLY);
			if (fd < 0) {
				fprintf(2, "find: cannot open %s\n", buf);
				exit(1);
			}
			close(0); // close stdin
			dup(fd);  // duplicate fd to stdin
			close(fd);
            exec(exec_cmd, exec_args);
            exit(0);
          } else {
            wait(0);
          }
        }
      }
    }
  }

  close(fd);
}

/**
 * find.c
 *
 * Usage: find PATH FILENAME
 */
int main(int argc, char *argv[]) {
  // Your code here.
  if (argc < 3) {
    fprintf(2, "Usage: find PATH FILENAME [-exec CMD]\n");
    exit(1);
  }
  if (argc > MAXARG) {
	fprintf(2, "find: too many arguments\n");
	exit(1);
  }

  char *path = argv[1];
  char *filename = argv[2];

  if (strcmp(argv[3], "-exec") == 0) {
    exec_flag = 1;
    exec_cmd = argv[4];
    exec_args = malloc((argc - 4 + 1) * sizeof(char *));
    for (int i = 4; i < argc; i++) {
      exec_args[i - 4] = argv[i];
    }
    exec_args[argc - 4] = 0; // Null-terminate the args array
    last_arg_index = argc - 4;
  }

  search_and_print(path, filename);
  return 0;
}