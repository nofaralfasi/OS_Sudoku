#ifndef SUDOKU_MAIN_H
#define SUDOKU_MAIN_H

#define UNK 2
#define PROCESSES 3
#define PIPES 4
#define SIZE 9
#define FDL 12

void forkAndAssign(pid_t *process, int **fd);

int initProcess(int i, int **fd);

void writeToFdFromTerminal(int **fd);

void writeToFdFromFile(char *filename, int **fd);

void sudokuChecker(int **fd, char *filePath);

int readFromResultPipe(int fd);

void printMatrix(int **mat);

void writeToPipe(int fd, int **matrix);

void waitForChildren();

#endif //SUDOKU_MAIN_H
