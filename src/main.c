#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <config.h>
#include <error_message.h>
#include <process_manager.h>

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
  
  pm_exit(pm);
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
