#!/bin/bash

# Check if a program name is provided as an argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <program_name>"
    exit 1
fi

# Compile the program
g++ "$1.cpp" glad.c -o "$1" -Lfolder -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the program
./"$1"