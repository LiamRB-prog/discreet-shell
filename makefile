# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Werror -std=c++17

# Source files and object files
SRC := main.cpp
OBJ := $(SRC:.cpp=.o)

# Output binary name
TARGET := main

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Optional: phony targets
.PHONY: all clean

