# Name of the executable
TARGET = connect

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -g

# Source files
SRCS = Connect4.c main.c Helper.c

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean compiled files
# clean:
# 	rm -f $(TARGET)
