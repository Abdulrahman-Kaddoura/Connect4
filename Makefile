# Name of the executable
TARGET = connect4
TEST_TARGET = testBots

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -g

# Source files
SRCS = Connect4.c main.c Helper.c AIMoves.c

# Default target
all: $(TARGET)

# Build the normal executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Build the test bots executable
$(TEST_TARGET): $(SRCS)
	$(CC) $(CFLAGS) -DTEST_BOTS $(SRCS) -o $(TEST_TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET) $(TEST_TARGET)
