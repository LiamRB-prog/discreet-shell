#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <process_manager.h>
#include <commands.h>

int c_delegate_cmd(ProcessManager* pm, char** argv) {
  if (strcmp(argv[0], "shout") == 0) {
    c_shout(pm, argv[1]);
    return 1;
  }
  if (strcmp(argv[0], "prm") == 0) {
    c_remove(pm, argv[1]);
    return 1;
  }

  return 0;
}

void c_shout(ProcessManager* pm, char* argv) {
  if (pm->processes[atoi(argv)] == NULL) return;
  if (pm->processes[atoi(argv)]->buf == NULL) return;

  printf("%s\n", pm->processes[atoi(argv)]->buf);
}

void c_remove(ProcessManager* pm, char* argv) {
  if (pm->processes[atoi(argv)] == NULL) return;
  if (pm->processes[atoi(argv)]->buf == NULL) return;

  pm_remove_proc(pm, atoi(argv));

  printf("Removed Process");
}