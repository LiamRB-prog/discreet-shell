#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARG_SIZE 1024
#define ARG_NUM 64
#define FILE_NAME "./main"

void interactive();
char** tokenize_input(char*, size_t*);
void read_from_pipe(int);
void error_message(int, char*);

int main(int argc, char** argv) {
	if (argc < 2) {
		interactive();
	}

	return 0;
}

void interactive() {
	while(true) {
		size_t size = ARG_SIZE;
		char* input = NULL;

		printf("dsh> ");

		if (getline(&input, &size, stdin) == -1) {
			error_message(EXIT_FAILURE ,"ERROR GETTING LINE");
		}

		size_t argc;
		char** argv = tokenize_input(input, &argc);

		if (argc == 0 || argv[0] == NULL) {
			free(input);
			free(argv);
			continue;
		}

		int fd[2];
		
		if (pipe(fd) == -1) {
			error_message(EXIT_FAILURE, "ERROR PIPING");
		}

		pid_t pid = fork();

		if (pid == 0) {
			close(fd[0]);
			dup2(0, fd[1]);

			execvp(argv[0], argv);
		}
		else {
			close(fd[1]);

			read_from_pipe(fd[0]);
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

	if (argv[argc-1][strlen(argv[argc-1])-1] == '\n') {
		argv[argc-1][strlen(argv[argc-1])-1] = '\0';
	}

	argv[argc] = NULL;
	*count = argc;
	return argv;
}

void read_from_pipe(int fd) {
	FILE* stream = fdopen(fd, "r");

	if (!stream) {
		error_message(EXIT_FAILURE, "ERROR OPENING STREAM");
	}

	char* buffer = NULL;
	size_t size = 0;

	while (getline(&buffer, &size, stream) != -1) {
		printf("%s", buffer);
	}

	free(buffer);
	fclose(stream);
}

void error_message(int code, char* message) {
	err(code, message);
	exit(code);
}