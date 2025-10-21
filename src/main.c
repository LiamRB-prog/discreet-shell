#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <config.h>
#include <error_message.h>
#include <process_manager.h>
#include <commands.h>

struct ProcessManager* pm = NULL;

void interactive();
char** tokenize_input(char*, size_t*);
char* read_pipe_buf(int);

int main(int argc, char** argv) {
  pm = pm_init();

	if (argc < 2) {
		interactive();
	}
  
  pm_exit(pm);
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

    	bool skip = c_delegate_cmd(pm, argv);

    	if (skip) continue;
		
    	pm_add_proc(pm, argv);

		for (size_t i = 0; i < argc; i++) {
			free(argv[i]);
		}

		free(argv);
		free(input);
	}
}

char** tokenize_input(char* line, size_t* count) {
	size_t argc = 0;
	char** argv = malloc(sizeof(char*) * ARG_NUM);

	char* token = strtok(line, " \t");

	while(token != NULL && argc < ARG_NUM) {
		argv[argc++] = strdup(token);
		token = strtok(NULL, " \t");
	}

	argv[argc] = NULL;
	*count = argc;
	return argv;
}
