#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <config.h>
#include <process.h>
#include <process_manager.h>
#include <error_message.h>

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

  if (pipe(proc->fd) == -1) {
    error_message(EXIT_FAILURE, "ERROR PIPING");
  }

  pid_t pid = fork();

  if (pid == 0) {
    proc->pid = getpid();
    close(proc->fd[0]);
    dup2(proc->fd[1], STDOUT_FILENO);

    run_proc(proc);
  }
  else {
    close(proc->fd[1]);
    proc->buf = read_pipe_buf(proc->fd[0]);

    int status;

    waitpid(proc->pid, &status, WNOHANG);
  }
}

void pm_remove_proc(ProcessManager* pm, int index) {
  if (pm->processes[index] == NULL) return;

  if (pm->processes[index]->buf != NULL) {
    free(pm->processes[index]->buf);
  }

  if (pm->processes[index]->command != NULL) {
    free(pm->processes[index]->command);
  }

  free(pm->process[index]);
  pm->processes[index] = NULL;
}

void pm_exit(ProcessManager* pm) {
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if (pm->processes[i] != NULL) {
      if (pm->processes[i]->buf != NULL) {
        free(pm->processes[i]->buf);
      }

      free(pm->processes[i]);
    }
  }

  free(pm->processes);
  free(pm);
}

char* read_pipe_buf(int fd) {
	FILE* stream = fdopen(fd, "r");

	if (!stream) {
		error_message(EXIT_FAILURE, "ERROR OPENING STREAM");
	}

	char* buffer = NULL;
	size_t size = 0;

	while (getline(&buffer, &size, stream) != -1);

	fclose(stream);

	return buffer;
}
