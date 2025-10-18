#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <sys/types.h>

typedef struct Process {
  pid_t pid;
  char* buf;
  char** command;
  int fd[2];
} Process;

Process* create_proc(char** argv);
void run_proc(Process* proc);

#endif
