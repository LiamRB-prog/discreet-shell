#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

Process create_proc(char** argv) {
  Process proc = {.command = argv};
  
  int fd[2];

  if (pipe(fd) == -1) {
    printf("ERROR PIPING");
    exit(1);
  }

  pid_t pid = fork();

  if (pid == 0) {
    proc.pid = getpid();

    close(fd[0]);
    dup2(fd[1], 1);

    execvp(argv[0], argv);
  }
  else {
    return proc;
  }

  return proc;
}

#endif
