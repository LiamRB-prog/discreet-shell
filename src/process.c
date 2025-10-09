#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <process.h>

Process* create_proc(char** argv) {
  Process* proc = (Process*)malloc(sizeof(Process));
  
  if (!proc) return NULL;

  proc->command = argv;

  int fd[2];

  if (pipe(fd) == -1) {
    printf("ERROR PIPING");
    return NULL;
  }

  pid_t pid = fork();

  if (pid == 0) {
    proc->pid = getpid();

    close(fd[0]);
    dup2(fd[1], 1);

    execvp(argv[0], argv);
    return proc;
  }
  else {
    return proc;
  }
}
