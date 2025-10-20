#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <process_manager.h>
#include <commands.h>

bool c_delegate_cmd(ProcessManager* pm, char** argv) {
  if (strcmp(argv[0], "shout") == 0) {
    c_shout(pm, argv[1]);
    return true;
  }

  return false;
}

void c_shout(ProcessManager* pm, char* argv) {
  if (pm->processes[strtol(argv)] == NULL) return;
  if (pm->processes[strtol(argv)]->buf == NULL) return;

  printf("%s\n", pm->processes[strtol(argv)]->buf);
}

void c_remove(ProcessManager* pm, char* argv) {
  if (pm->processes[strtol(argv)] == NULL) return;
  if (pm->processes[strtol(argv)]->buf == NULL) return;

  // add functionality

  printf("Removed Process")
}