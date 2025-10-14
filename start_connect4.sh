#!/bin/bash
clear

# Go to the project folder
cd /root/Connect4_CMPS241project || exit 1

# Compile the source code every time
gcc connect4.c -o connect

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running Connect 4..."
    ./connect
else
    echo "Compilation failed. Please check connect4.c"
fi
