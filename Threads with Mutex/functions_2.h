#ifndef FUNCTIONS_2_H_
#define FUNCTIONS_2_H_

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

#define SUDOKU_LENGTH 9
#define NUM_THREADS 12
#define NUM_OF_TASK 27

typedef struct {
	int* missionIndex;
	int** Allmiss;				 		//all mission arrays
	int* my_errors;						//shared answer for sum of all elements
	int* number_of_running_threads;
	pthread_mutex_t *mutex;				// mutex to synchronize a write to sum
	pthread_cond_t* tasksCond;
} thread_data;

typedef struct {
	int mat[SUDOKU_LENGTH][SUDOKU_LENGTH];
} Sudoku;

void print_result(int error, char** argv);
void end_thread(pthread_mutex_t* mutex, int* number_of_running_threads,
		pthread_cond_t* tasksCond);
int get_index_of_mission(int* missionIndex, pthread_mutex_t* mutex);
void mutex_and_attr(pthread_mutex_t* res_mutex, pthread_attr_t* attr);
void* threadWork(void *threadarg);
int checkArr(int* arr);
void missions(int** allMiss, Sudoku* s);
void getSudoku(int argc, Sudoku* s, char** argv, FILE* f);
void readSudokuFromUser(Sudoku* s);
int readSudokuFromFile(FILE *f, char* fileName, Sudoku* s);
void printSudoku(Sudoku* s);
char* sudokuFromintToString(int mat[SUDOKU_LENGTH][SUDOKU_LENGTH]);
void copySudokuRow(int dest[SUDOKU_LENGTH], int src[SUDOKU_LENGTH]);

#endif /* FUNCTIONS_2_H_ */
