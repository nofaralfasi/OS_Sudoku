#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"

int main(int argc, char *argv[]) {
    pid_t process[PROCESSES];
    int fd[PIPES][UNK];
    char *filePath = "";

    for (int i = 0; i < PIPES; ++i) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        } else {
            printf("fd %d: %d , %d   ", i, fd[i][0], fd[i][1]);
        }
    }

    if (argv[1] == NULL) {
        writeToFdFromTerminal(fd);
    } else {
        filePath = argv[1];
        writeToFdFromFile(filePath, fd);
    }

    forkAndAssign(process, fd);
    waitForChildren();
    sudokuChecker(fd, filePath);

    return 0;
}

void forkAndAssign(pid_t *process, int **fd) {
    for (int i = 0; i < PROCESSES; ++i) {
        process[i] = fork();

        if (process[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);

        } else if (process[i] == 0) {
            close(fd[i][1]);
            if (initProcess(i, fd) == -1) {
                fprintf(stderr, "Init process failed.");
                exit(1);
            }

        } else {
            continue;
        }
    }
}

int initProcess(int i, int **fd) {
    char *execFile = ".\sudoku";
    char *part_to_check;
    char fdRead[FDL], fdWrite[FDL];

    if (i == 0) {
        part_to_check = "rows";
    } else if (i == 1) {
        part_to_check = "cols";
    } else {
        part_to_check = "sub-matrices";
    }

    sprintf(fdRead, "%i", fd[i][0]);
    sprintf(fdWrite, "%i", fd[PIPES - 1][1]);

    char *args[] = {execFile, part_to_check, fdRead, fdWrite, NULL};
    return execvp(args[0], args);
}

void writeToFdFromFile(char *filename, int **fd) {
    int i, j;
    int **mat = (int **) malloc(SIZE * sizeof(int *));
    for (i = 0; i < SIZE; ++i)
        mat[i] = (int *) malloc(SIZE * sizeof(int));
    char c;

    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
        fprintf(stderr, "File doesn't exist.");

    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            fscanf(file, "%d%c", &mat[i][j], &c);
        }
    }

    printMatrix(mat);
    fclose(file);

    for (i = 0; i < PROCESSES; ++i) {
        writeToPipe(fd[i][1], mat);
        close(fd[i][1]);
    }
}

void writeToFdFromTerminal(int **fd) {
    int i, j;
    char c;
    int **mat = (int **) malloc(SIZE * sizeof(int *));

    for (i = 0; i < SIZE; ++i)
        mat[i] = (int *) malloc(SIZE * sizeof(int));

    for (i = 0; i < SIZE; ++i) {
        printf("\nEnter the numbers in row %d:\n", i + 1);
        for (j = 0; j < SIZE; ++j)
            scanf("%d%c", &mat[i][j], &c);
    }

    printMatrix(mat);

    for (i = 0; i < PROCESSES; ++i) {
        writeToPipe(fd[i][1], mat);
        close(fd[i][1]);
    }
}

void writeToPipe(int fd, int **matrix) {
    int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            write(fd, &matrix[i][j], sizeof(int));
        }
    }
}


void waitForChildren() {
    for (int i = 0; i < PROCESSES; i++) {
        wait(NULL);
    }
}

void sudokuChecker(int **fd, char *filePath) {
    int result = readFromResultPipe(fd[PIPES - 1][0]);

    if (result == PROCESSES) {
        printf("\nSudoku at file '%s' is legal\n", filePath);
    } else {
        printf("\nSudoku at file '%s' is not legal\n", filePath);
    }
}

int readFromResultPipe(int fd) {
    int i, result = 0, tmp = 0;
    for (i = 0; i < PROCESSES; i++) {
        read(fd, &tmp, sizeof(int));
        if (tmp > 0)
            result++;
    }
    close(fd);
    return result;
}

void printMatrix(int **mat) {
    printf("\nSudoku Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    printf("\n");
}
