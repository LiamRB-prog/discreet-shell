#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "process_manager.h"

#define ARG_SIZE 1024
#define ARG_NUM 64

struct ProcessManager* pm = NULL;

void interactive(char*);
char** tokenize_input(char*, size_t*);
char* read_pipe_buf(int);
void error_message(int, char*);

int main(int argc, char** argv) {
  pm = pm_init();

	if (argc < 2) {
    char* buf = NULL;
		interactive(buf);
	}

	return 0;
}

void interactive(char* buf) {
	while(true) {
		size_t size = ARG_SIZE;
		char* input = NULL;

		printf("dsh> ");

		if (getline(&input, &size, stdin) == -1) {
			error_message(EXIT_FAILURE ,"ERROR GETTING LINE");
		}

		if (input[strlen(input)-1] == '\n') {
			input[strlen(input)-1] = '\0';
		}

		size_t argc;
		char** argv = tokenize_input(input, &argc);

		if (argc == 0 || argv[0] == NULL) {
			free(input);
			free(argv);
			continue;
		}
    else if (strcmp(argv[0], "show") == 0) {
      if (buf != NULL) {
        printf("%s", buf);
      }
      else {
        printf("Buffer is empty.\n");
      }

      continue;
    }

		int fd[2];
		
		if (pipe(fd) == -1) {
			error_message(EXIT_FAILURE, "ERROR PIPING");
		}

    pm_add_proc(pm, argv, argc);

		pid_t pid = fork();

		if (pid == 0) {
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);

			
		}
		else {
			close(fd[1]);

			buf = read_pipe_buf(fd[0]);

			int status = 1;
			waitpid(pid, &status, 0);
		}

		for (size_t i = 0; i < argc; i++) {
			free(argv[i]);
		}

		free(argv);
		free(input);
	}
}

char** tokenize_input(char* line, size_t* count) {
	size_t argc = 0;
	char** argv = (char**)malloc(sizeof(char*) * ARG_NUM);

	char* token = strtok(line, " \t");

	while(token != NULL && argc < ARG_NUM) {
		argv[argc++] = strdup(token);
		token = strtok(NULL, " \t");
	}

	argv[argc] = NULL;
	*count = argc;
	return argv;
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

void error_message(int code, char* message) {
	err(code, message);
	exit(code);
}
