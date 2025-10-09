# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -Iinclude

# Source and object files
SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)

# Output binary
TARGET := bin/shell

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation step
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build (adds -g)
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Clean artifacts
clean:
	rm -rf build bin

.PHONY: all clean debug

