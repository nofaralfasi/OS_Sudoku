#ifndef HEADER_MAINFUNC_H_
#define HEADER_MAINFUNC_H_
#include "config.h"
#include <pthread.h>

void settingUpThreads(pthread_t thread_rows[MAT_SIZE], pthread_t thread_cols[MAT_SIZE],
                     pthread_t thread_subMat[MAT_SIZE], GAMESTATE *shared_gameState);
void readFromStandardInput(int matrix[MAT_SIZE][MAT_SIZE], GAMESTATE *shared_gameState);
void readMatrixFromFile(int matrix[MAT_SIZE][MAT_SIZE], char *filename, GAMESTATE *shared_gameState);
void sudokuChecker(GAMESTATE *shared_gameState);
void printSudokuMatrix(int matrix[MAT_SIZE][MAT_SIZE]);

#endif /* HEADER_MAINFUNC_H_ */
