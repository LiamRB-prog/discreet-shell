#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARG_SIZE 1024
#define ARG_NUM 64
#define FILE_NAME "./main"

void interactive();
char** tokenize_input(char* line, size_t* count);

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

		if (!getline(&input, &size, stdin)) {
			perror("ERROR GETTING LINE");
			exit(1);
		}

		size_t argc;
		char** argv = tokenize_input(input, &argc);

		if (argc == 0 || argv[0] == NULL) {
			free(input);
			free(argv);
			continue;
		}

		int fd[2];
		
		if (!pipe(fd)) {
			perror("ERROR PIPING");
			exit(1);
		}

		pid_t pid = fork();

		if (pid == 0) {
			close(fd[0]);
			dup2(0, fd[1]);

			execvp(argv[0], argv);
		}
		else {
			close(fd[1]);

			char* buffer = NULL;

			while (getline(&buffer, &size, fdopen(fd[0], "r")) != 0) {
				printf("%s\n", buffer);
			}

			free(buffer);
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