
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS	27
#define SUDOKU_LENGTH 9

typedef enum
{
	false = 0, true = 1
} boolean;

typedef struct
{
	int mat[SUDOKU_LENGTH][SUDOKU_LENGTH];
} Sudoku;

typedef struct
{
	int arr[SUDOKU_LENGTH];
	int res;
} thread_data;



void get_sudoku(int argc, Sudoku* s, char** argv, FILE* f);
void copySudokuCols(thread_data threads_data[NUM_THREADS], int t,
		const Sudoku* s);
void copySudokuMinors(thread_data threads_data[NUM_THREADS], int t,
		const Sudoku* s);
void checkAnswer(int answer, int ans[NUM_THREADS], char** argv);
void readSudokuFromUser(Sudoku* s);
void* checkArr(void *threadarg);
void readSudokuFromUser(Sudoku* s);
int readSudokuFromFile(FILE *f, char* fileName, Sudoku* s);
void printSudoku(Sudoku* s);
char* sudokuFromintToString(int mat[SUDOKU_LENGTH][SUDOKU_LENGTH]);
void copySudokuRow(int dest[SUDOKU_LENGTH], int src[SUDOKU_LENGTH]);
#endif /* FUNCTIONS_H_ */
