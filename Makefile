# Directories
SRC_DIR = src/
INCLUDE_DIR = include/
BUILD_DIR = build/

# Source files (assuming they're in src/)
SOURCES = $(wildcard $(SRC_DIR)*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)%.c=$(BUILD_DIR)%.o)

# Test files
TEST_SOURCES = $(wildcard tests.c)
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(BUILD_DIR)%.o)
TEST_DEPS = $(filter-out $(BUILD_DIR)main.o, $(OBJECTS))

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR) -g
DEBUG_CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR) -g -O0 -DDEBUG
TARGET = qsim
DEBUG_TARGET = qsim_debug
TEST_TARGET = tests

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Main executable
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Debug executable
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CC) $(DEBUG_CFLAGS) $(OBJECTS) -o $(DEBUG_TARGET)

# Test executable
$(TEST_TARGET): $(BUILD_DIR) $(TEST_OBJECTS) $(TEST_DEPS)
	$(CC) $(TEST_OBJECTS) $(TEST_DEPS) -o $(TEST_TARGET)

# Compile source files
$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files (if they're in root directory)
$(BUILD_DIR)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(DEBUG_TARGET) $(TEST_TARGET)

clean_tests:
	rm -f $(TEST_TARGET) $(TEST_OBJECTS)

# Phony targets
.PHONY: all debug clean clean_tests