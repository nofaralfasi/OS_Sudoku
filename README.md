# OS_Sudoku

A multi process Sudoku checking program.
The program's main purpose is to check if a given matrix is a valid Sudoku solution.

## Instructions:

(Using gcc compiler to run the code)

1. Open linux terminal on the main directory.

2. To execute the Makefile run the command: make -f Makefile.mak

This command compiles the relevant .c files and creates executable files of the program.

3. To execute the Sudoku program run the command: ./executable_file [text_file] (optional - a text file with a Sudoku matrix)

If no text file is given: The program reads the Sudoku matrix from the user in the terminal.

## Implementations
### Pipes Execvp
Uses three child processes each one has a to send the data. The processes use 'execvp' to call another process to run.

### Shared Memory
Uses three child processes and on a shared memory.

### Multi Threads
Each thread is responsible for checking different elements of the Sudoku matrix (rows, columns and sub-matrices).

### Threads with Mutex
All threads have access to the same data. Synchronization is made by using 'mutex' and condition variable.
