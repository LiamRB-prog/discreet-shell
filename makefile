# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror
DEBUG_FLAGS := -Wall -Werror -DDEBUG -g

# Source files and object files
SRC := main.c
OBJ := $(SRC:.c=.o)

# Output binary name
TARGET := main
DEBUG_TARGET := main_debug

# Default rule
all: $(TARGET)

# Linking (normal build)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compiling (normal build)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: clean
	$(CC) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET) $(DEBUG_TARGET)

.PHONY: all clean debug

