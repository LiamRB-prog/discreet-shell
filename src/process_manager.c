#include <stdio.h>
#include <config.h>
#include <process.h>
#include <process_manager.h>

ProcessManager* pm_init() {
  ProcessManager* pm = malloc(sizeof(ProcessManager));

  for(int i = 0; i < MAX_PROCESSES; i++) {
    pm->processes[i] = NULL;
  }

  return pm;
}

void pm_add_proc(ProcessManager* pm, char** argv) {
  bool pmFull = true;
  int index = 0;

  for(; index < MAX_PROCESSES; index++) {
    if(pm->processes[index] == NULL) {
      pmFull = false;
      break;
    }
  }

  if (pmFull) {
    printf("The process manager is full.");
    return;
  }

  Process* proc = create_proc(argv);
  pm->processes[index] = proc;
}

void pm_exit(ProcessManager* pm) {
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if (pm->processes[i] != NULL) free(pm->processes[i]);
  }

  free(pm);
}
