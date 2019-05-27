#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "main.h"


int main(int argc, char *argv[]) {
    int matrix[MAT_SIZE][MAT_SIZE];
    pid_t process[PROCESS_NUM];
    char *filePath;

    SHARED *shared_struct_obj = mmap(NULL, MEM_LENGTH, PROT_READ | PROT_WRITE,
                                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_struct_obj == MAP_FAILED) {
        perror("Mmap failed\n");
        exit(EXIT_FAILURE);
    }
    //matrix exists
    if (argv[1] != NULL) {
        filePath = argv[1];
        read_from_file_and_write_matrix(matrix, filePath, shared_struct_obj);
    } else {
        //file doens't exist
        read_from_terminal_and_write_matrix(matrix, shared_struct_obj);
    }

    fork_and_assign(process, shared_struct_obj);
    wait_for_children();
    check_sudoku(filePath, shared_struct_obj);
    munmap(shared_struct_obj, MEM_LENGTH);

    return 0;
}

void fork_and_assign(pid_t *process, SHARED *shared_struct_obj) {
    for (int i = 0; i < PROCESS_NUM; i++) {
        process[i] = fork();

        if (process[i] == 0) {
            init_process(i, shared_struct_obj);

        } else if (process[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);

        } else {
            continue;
        }
    }
}

void init_process(int i, SHARED *shared_struct_obj) {
    int matrix[MAT_SIZE][MAT_SIZE];
    int result = 0;

    memcpy(matrix, shared_struct_obj->solution, SOLUTION_SIZE);

    if (i == 0) {
        result = checkMatrixRows(matrix);
    } else if (i == 1) {
        result = checkMatrixCols(matrix);
    } else {
        result = checkSubMatrix(matrix);
    }

    shared_struct_obj->status[i] = result;
    exit(EXIT_SUCCESS);
}

void read_from_file_and_write_matrix(int matrix[MAT_SIZE][MAT_SIZE], char *filename, SHARED *shared_struct_obj) {
    int i, j;
    char c;

    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
        fprintf(stderr, "File is missing.");

    for (i = 0; i < MAT_SIZE; ++i) {
        for (j = 0; j < MAT_SIZE; ++j) {
            fscanf(file, "%d%c", &matrix[i][j], &c);
        }
    }

    print_matrix(matrix);
    memcpy(shared_struct_obj->solution, matrix, SOLUTION_SIZE);

    fclose(file);
}

void read_from_terminal_and_write_matrix(int matrix[MAT_SIZE][MAT_SIZE], SHARED *shared_struct_obj) {
    int i, j;
    char c;

    for (i = 0; i < MAT_SIZE; ++i) {
        printf("\nEnter row number %d (num by num):\n", i + 1);

        for (j = 0; j < MAT_SIZE; ++j) {
            scanf("%d%c", &matrix[i][j], &c);
        }
    }

    print_matrix(matrix);
    memcpy(shared_struct_obj->solution, matrix, SOLUTION_SIZE);
}

void check_sudoku(char *filePath, SHARED *shared_struct_obj) {
    int final_result = 0;
    for (int i = 0; i < PROCESS_NUM; ++i) {
        final_result += shared_struct_obj->status[i];
    }

    if (final_result == LEGAL_SEQUENCE) {
        printf("\nSudoku at file '%s' is legal\n", filePath);
    } else {
        printf("\nSudoku at file '%s' is not legal\n", filePath);
    }
}

void print_matrix(int matrix[MAT_SIZE][MAT_SIZE]) {
    printf("\nYour matrix is:\n");
    for (int i = 0; i < MAT_SIZE; i++) {
        for (int j = 0; j < MAT_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void wait_for_children() {
    for (int i = 0; i < PROCESS_NUM; i++) {
        wait(NULL);
    }
}


// The algorithm: we keep a checking list, each value in the row/col/subMat represent an index in the checklist.
// We move through each row/col/subMat and upgrade the index in the checklist accordingly.
// If we find a value not in the range of 1-9 we disqualify the sudoku.
// If after iterating through each row/col/subMat we find any index in the checklist that it's not 1 we disqualify the sudoku.

int checkMatrixRows(int matrix[MAT_SIZE][MAT_SIZE]) {
    printf("Going through matrix's rows\n");
    int result = 1;
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < MAT_SIZE; ++i) {
        for (int j = 0; j < MAT_SIZE; ++j) {
            if (matrix[i][j] < BASE && matrix[i][j] > 0) {
                ++check_list[matrix[i][j] - 1];
            } else {
                result = 0;
                break;
            }
        }
        for (int k = 0; k < MAT_SIZE; ++k) {
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            check_list[k] = 0;
        }
    }
    return result;
}

int checkMatrixCols(int matrix[MAT_SIZE][MAT_SIZE]) {
    printf("Going through matrix's cols\n");
    int result = 1;
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int j = 0; j < MAT_SIZE; ++j) {
        for (int i = 0; i < MAT_SIZE; ++i) {
            //legal values
            if (matrix[i][j] < BASE && matrix[i][j] > 0) {
                ++check_list[matrix[i][j] - 1];
            } else {
                result = 0;
                break;
            }
        }
        for (int k = 0; k < MAT_SIZE; ++k) {
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            check_list[k] = 0;
        }
    }
    return result;
}

int checkSubMatrix(int matrix[MAT_SIZE][MAT_SIZE]) {
    printf("Going through matrix's sub-matrices\n");
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int result = 1;
    int offsetX = 0;
    int offsetY = 0;
    while (offsetY <= OFFSET) {
        for (int i = 0; i < DIFF; ++i) {
            for (int j = 0; j < DIFF; ++j) {
                if (matrix[i + offsetY][j + offsetX] < BASE && matrix[i + offsetY][j + offsetX] > 0) {
                    ++check_list[matrix[i + offsetY][j + offsetX] - 1];
                } else {
                    result = 0;
                    break;
                }
            }

        }
        offsetX += DIFF;
        if (offsetX > OFFSET) {
            offsetY += DIFF;
            offsetX = 0;
        }

        for (int k = 0; k < MAT_SIZE; ++k) {
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            check_list[k] = 0;
        }
    }
    return result;
}

