#!/bin/bash
clear

# Go to the project folder
cd /root/Connect4 || exit 1

# Compile the source code every time
gcc -Wall -Werror -g Connect4.c main.c Helper.c AIMoves.c Connect4_server.c Connect4_client.c -o connect4

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running Connect 4..."
    ./connect4
else
    echo "Compilation failed. Please check Connect4.c"
fi
