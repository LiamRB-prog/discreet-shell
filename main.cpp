#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv) {
	int fd[2]; // fd[0] is input, fd[1] is output
	pid_t childProcess;

	pipe(fd);

	childProcess = fork();

	if (childProcess == -1) {
		perror("fork");
		exit(1);
	}

	if (childProcess == 0) {
		// This is the child process
		

		close(fd[0]);

		int x;
		scanf("%d", &x);
		write(fd[1], &x, sizeof(int));
		close(fd[1]);	
	}
	else {
		// This is the parent process

		close(fd[1]);

		int y;		
		read(fd[0], &y, sizeof(int));
		close(fd[0]);

		printf("%d\n", y);
	}

	return 0;
}
