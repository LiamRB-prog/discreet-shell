#include "process.h"
#include "process_manager.h"

ProcessManager* pm_init() {
  ProcessManager* pm;

  for(int i = 0; i < sizeof(pm->processes)/sizeof(Process*); i++) {
    pm->processes[i] = NULL;
  }

  return pm;
}

void pm_add_proc(ProcessManager* pm, char** argv) {
  bool pmFull = true;
  int index = 0;

  for(; index < sizeof(pm->processes)/sizeof(Process*); index++) {
    if(pm->processes[index] == NULL) {
      pmFull = false;
      break;
    }
  }

  if (pmFull) {
    printf("The process manager is full.");
    return;
  }

  Process proc = create_proc(argv);
  pm->processes[index] = proc;
}
