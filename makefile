# Compiler and flags
CXX := gcc
CXXFLAGS := -Wall -Werror

# Source files and object files
SRC := main.c
OBJ := $(SRC:.c=.o)

# Output binary name
TARGET := main

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Optional: phony targets
.PHONY: all clean

