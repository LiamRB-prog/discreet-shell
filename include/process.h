#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <sys/types.h>

typedef struct Process {
  pid_t pid;
  char* buf;
  char** command;
} Process;

Process* create_proc(char** argv);
char* read_pipe_buf(int fd);

#endif
