#ifndef SUDOKU_SUDOKUCHECKER_H
#define SUDOKU_SUDOKUCHECKER_H

#define DIFF 3
#define OFFSET 6
#define SIZE 9
#define BASE 10

void checkMatrixRows(int **matrix);

void check_cols(int matrix[SIZE][SIZE]);

void checkSubMatrix(int **matrix);

static int result = 1;

#endif //SUDOKU_SUDOKUCHECKER_H
