#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../header/mainFunc.h"
#include "../header/threadFunc.h"
#include "../header/config.h"
#include <pthread.h>

//make -f Makefile.mak

int main(int argc, char *argv[]) {
	int matrix[MAT_SIZE][MAT_SIZE];
	pthread_t rowsThreads[MAT_SIZE];
	pthread_t colsThreads[MAT_SIZE];
	pthread_t subMatricesThreads[MAT_SIZE];
	GAMESTATE shared_gameState;
	char* filePath = "/home/HP/HW2/demo.txt";

	if (argv[1] != NULL) {
		filePath = argv[1];
		readMatrixFromFile(matrix, filePath, &shared_gameState);
	} else {
		readFromStandardInput(matrix, &shared_gameState);
	}

	settingUpThreads(rowsThreads, colsThreads, subMatricesThreads, &shared_gameState);
	sudokuChecker(&shared_gameState);
//	sudokuChecker(filePath, &shared_gameState);

	return 0;
}
