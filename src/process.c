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

  int fd[2];

  if (pipe(fd) == -1) {
    error_message(EXIT_FAILURE, "ERROR PIPING");
  }

  pid_t pid = fork();

  if (pid == 0) {
    proc->pid = getpid();

    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);

    execvp(argv[0], argv);
    return proc;
  }
  else {
    close(fd[1]);
    char* buf = read_pipe_buf(fd[0]);

    int status;

    waitpid(proc->pid, &status, WNOHANG);
    strcpy(buf, proc->buf);

    return proc;
  }
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
