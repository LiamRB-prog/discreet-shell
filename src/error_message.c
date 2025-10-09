#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <error_message.h>

void error_message(int code, char* message) {
	err(code, message);
	exit(code);
}
