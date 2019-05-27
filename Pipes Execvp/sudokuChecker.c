#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudokuChecker.h"


int main(int argc, char *argv[]) {
    // convert a string to a long integer
    int fdRead = (int) strtoll(argv[2], NULL, BASE);
    int fdWrite = (int) strtoll(argv[3], NULL, BASE);
    int matrix[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            read(fdRead, &matrix[i][j], sizeof(int));
        }
    }
    close(fdRead);

    // if argv[1] == "rows"
    if (strcmp(argv[1], "rows") == 0) {
        checkMatrixRows(matrix);

        // if argv[1] == "cols"
    } else if (strcmp(argv[1], "cols") == 0) {
        check_cols(matrix);

        // if argv[1] == "subMat"
    } else if (strcmp(argv[1], "sub-matrices") == 0) {
        checkSubMatrix(matrix);

    } else {
        exit(1);
    }

    write(fdWrite, &result, sizeof(int));
    close(fdWrite);

    return 0;
}

// The check-list is initialized with all '0'. Then we go through each element in the matrix that needs to be checked.
// The elements needs to be checked are divide to 3 groups - rows, cols, and sub-matrices.
// In each element we go through its 9 digits. Each digit we encounter in is an index in our check-list.
// If the value on that index in our check-list is set to '1' - that means there is this element contains exactly one number equals to that index

void checkMatrixRows(int **matrix) {
    printf("Going through matrix's rows\n");
    // initializing the check-list with all '0'
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (matrix[i][j] < BASE && matrix[i][j] > 0) {
                // changes the value of the check-list in the index that is equals to the current value of the matrix
                ++(check_list[(matrix[i][j]) - 1]);
            } else {
                result = 0;
                break;
            }
        }

        // for each row of the matrix we go through our check-list to insure all index were set to '1'
        for (int k = 0; k < SIZE; ++k) {
            // if the check-list contains a value different than '1' we exit the loop since the sudoku is invalid
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            // preparing for next row checking
            check_list[k] = 0;
        }

    }
}

void check_cols(int matrix[SIZE][SIZE]) {
    printf("Going through matrix's cols\n");
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE; ++i) {
            if (matrix[i][j] < BASE && matrix[i][j] > 0) {
                ++check_list[matrix[i][j] - 1];
            } else {
                result = 0;
                break;
            }
        }
        for (int k = 0; k < SIZE; ++k) {
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            check_list[k] = 0;
        }
    }

}

void checkSubMatrix(int **matrix) {
    printf("Going through matrix's sub-matrices\n");
    int check_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int offsetX = 0;
    int offsetY = 0;

    while (offsetY <= OFFSET) {
        for (int i = 0; i < DIFF; ++i) {
            for (int j = 0; j < DIFF; ++j) {
                if ((matrix[i + offsetY][j + offsetX] < BASE) && (matrix[i + offsetY][j + offsetX] > 0)) {
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

        for (int k = 0; k < SIZE; ++k) {
            if (check_list[k] != 1) {
                result = 0;
                break;
            }
            check_list[k] = 0;
        }
    }
}


