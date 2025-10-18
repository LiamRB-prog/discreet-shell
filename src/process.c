#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <error_message.h>
#include <process.h>

Process* create_proc(char** argv) {
  Process* proc = malloc(sizeof(Process));
  
  if (!proc) return NULL;

  proc->command = argv;
  proc->buf = NULL;

  return proc;
}

void run_proc(Process* proc) {
  if (!proc->command) return;
  execvp(proc->command[0], proc->command);
}
