#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <sys/types.h>

#define MAX_PROCESSES 32

struct Process;

typedef struct ProcessManager {
  struct Process* processes[MAX_PROCESSES];
}

struct ProcessManager* pm_init();
void pm_add_proc(ProcessManager* pm, char** argv);

#endif
