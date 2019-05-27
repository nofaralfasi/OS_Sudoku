# OS_Sudoku

A multi process sudoku checking program.
The app's purpose is to decide if a given matrix is valid sudoku solution.

## Instructions:
To run with gcc compiler:

Open linux terminal in the relevant directory.

To compile the Makefile run the command: make -f Makefile.mak

To execute the Sudoku program run the command: ./v1 [filname] (optional - a text file with a sudoku matrix)

If no file is given: The program reads the Sudoku matrix from standard input on terminal.

## Implementations
### Pipes Execvp
Uses 3 child processes and a pipe for each one to send them the data, execvp to run another app on each one.

### Shared Memory
Uses 3 child processes and a shared memory.

### Multi Threads
Uses 3 threads. Each one is responsible for a different part.

### Threads with Mutex
Uses 3 threads which are responsible for the whole test, have access to the same data. Synchronization was taken care with mutex and condition variable.
