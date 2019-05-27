#ifndef MUTEX_MULTI_THREADS_H_
#define MUTEX_MULTI_THREADS_H_

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include "multiThreads.h"

#define SIZE 9
#define MUTEX_THRD_NUM 13
#define TASKS 27

typedef struct {
	int** tasks;
	int* taskIndex;
	int* wrongs;
	int* currNumOfThreads;
	pthread_mutex_t *mutex;
	pthread_cond_t* tasksCond;
} mutex_thread_info;

void printSudokuResult(int err, char** argv);
void finishThread(pthread_mutex_t* mutex, int* currNumOfThreads,
		pthread_cond_t* tasksCond);
int getTaskIndex(int* taskIndex, pthread_mutex_t* mutex);
void setTasks(int** tasks, sudoku_mat* s);

void mutex_and_attr(pthread_mutex_t* res_mutex, pthread_attr_t* attr);
void* threadTask(void *arg);
int mutexArrChecker(int* a);

void copySudokuRow(int dest[SIZE], int src[SIZE]);
void readFromStandardInput(sudoku_mat* s);
boolean readMatrixFromFile(FILE *f, char* fileName, sudoku_mat* s);
void printSudokuMatrix(sudoku_mat* s);
void getSudokuMatrix(int argc, sudoku_mat* s, char** argv, FILE* f);

#endif /* MUTEX_MULTI_THREADS_H_ */
