#ifndef MULTI_THREADS_H_
#define MULTI_THREADS_H_

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

#define THRD_NUM 27
#define SIZE 9

typedef enum {
false = 0, true = 1
}boolean;

typedef struct {
int mat[SIZE][SIZE];
} sudoku_mat;

typedef struct {
int arr[SIZE];
int res;
} thread_info;

void getSudokuMatrix(int argc, sudoku_mat* s, char** argv, FILE* f);
void readFromStandardInput(sudoku_mat* s);
void printSudokuMatrix(sudoku_mat* s);
boolean readMatrixFromFile(FILE *f, char* fileName, sudoku_mat* s);

void copySudokuCols(thread_info threads_info[THRD_NUM], int t, const sudoku_mat* s);
void copySudokuRow(int dest[SIZE], int src[SIZE]);
void copySudokuMinors(thread_info threads_info[THRD_NUM], int t, const sudoku_mat* s);
void checkSudokuResult(int answer, int ans[THRD_NUM], char** argv);
void* checkArr(void *arg);

#endif /* MULTI_THREADS_H_ */
