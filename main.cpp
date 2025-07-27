#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARG_SIZE 1024
#define ARG_NUM 64

void interactive();
char** tokenize_input(char* line, size_t* count);

int main(int argc, char** argv) {
	if (argc < 2) {
		interactive();
	}

	return 0;
}

void interactive() {
	printf("Welcome to Disscreet Shell interactive mode!\n");

	while(true) {
		size_t size = ARG_SIZE;
		char* input = NULL;

		printf("dsh> ");

		if (!getline(&input, &size, stdin)) {
			printf("ERROR GETTING LINE");
			exit(1);
		}

		size_t argc;
		char** argv = tokenize_input(input, &argc);

		pid_t pid = fork();

		if (pid == 0) {
			execvp(argv[0], argv);

			exit(0);
		}
		else {
			int status;
			waitpid(pid, &status, 0);
		}

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