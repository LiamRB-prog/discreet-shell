#ifndef COMMANDS_H
#define COMMANDS_H

#include <process_manager.h>

int c_delegate_cmd(ProcessManager* pm, char** argv);
void c_shout(ProcessManager* pm, char* argv);
void c_remove(ProcessManager* pm, char* argv);

#endif
