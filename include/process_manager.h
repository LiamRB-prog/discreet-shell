#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <sys/types.h>
#include <config.h>

struct Process;

typedef struct ProcessManager {
  struct Process* processes[MAX_PROCESSES];
} ProcessManager;

ProcessManager* pm_init();
void pm_add_proc(ProcessManager* pm, char** argv);
void pm_remove_proc(ProcessManager* pm, int index);
void pm_exit(ProcessManager* pm);

char* read_pipe_buf(int fd);

#endif
