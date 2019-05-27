#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "mutexMultiThreads.h"

void printSudokuResult(int err, char** argv) {
	printf("\n");
	if (err == 0) {
		printf("%s is legal\n", argv[1]);
	} else {
		printf("%s is not legal\n", argv[1]);
	}
}

void mutex_and_attr(pthread_mutex_t* res_mutex, pthread_attr_t* attr) {
	pthread_mutex_init(res_mutex, NULL);
	pthread_attr_init(attr);
	pthread_attr_setdetachstate(&*attr, PTHREAD_CREATE_JOINABLE);
}

void getSudokuMatrix(int argc, sudoku_mat* s, char** argv, FILE* f) {
	if (argc < 2) {
		readFromStandardInput(&*s);
	}
	char* filename = argv[1];
	if (readMatrixFromFile(f, filename, &*s) == -1) {
		perror("read from file");
		exit(-1);
	}
}

void finishThread(pthread_mutex_t* mutex, int* currNumOfThreads,
		pthread_cond_t* tasksCond) {
	pthread_mutex_lock(mutex);
	(*currNumOfThreads)++;
	pthread_mutex_unlock(mutex);
	pthread_cond_signal(tasksCond);
}

void* threadTask(void *arg) {
	int currTask;
	int currRes = 0;
	int* taskIndex;
	int* currNumOfThreads;
	int* fatherRes;
	int** allTasks;
	pthread_cond_t* tasksCond;
	pthread_mutex_t* mutex;
	taskIndex = ((mutex_thread_info *) arg)->taskIndex;
	fatherRes = ((mutex_thread_info *) arg)->wrongs;
	allTasks = ((mutex_thread_info *) arg)->tasks;
	mutex = ((mutex_thread_info *) arg)->mutex;
	tasksCond = ((mutex_thread_info *) arg)->tasksCond;
	currNumOfThreads =
			((mutex_thread_info *) arg)->currNumOfThreads;
	while ((currTask = getTaskIndex(taskIndex, mutex)) < TASKS) {
		if (*fatherRes != 0) {
			finishThread(mutex, currNumOfThreads, tasksCond);
			pthread_exit(NULL);
		}

		currRes = mutexArrChecker(allTasks[currTask]);
		if (currRes == 0) {
			*fatherRes += 1;
		}

	}
	finishThread(mutex, currNumOfThreads, tasksCond);
	pthread_exit(NULL);

}

int getTaskIndex(int* taskIndex, pthread_mutex_t* mutex) {
	pthread_mutex_lock(mutex);
	(*taskIndex)++;
	pthread_mutex_unlock(mutex);
	return (*taskIndex) - 1;
}

int mutexArrChecker(int* a) {
	int check[SIZE] = { 0 };
	for (int i = 0; i < SIZE; ++i) {
		int tmp = a[i];
		if (!check[tmp - 1]) {
			check[tmp - 1] = a[i];
		} else {
			return 0;
		}
	}
	for (int j = 0; j < SIZE; ++j) {
		if (!check[j]) {
			return 0;
		}
	}
	return 1;
}

void setTasks(int** tasks, sudoku_mat* s) {
	for (int t = 0; t < TASKS; ++t) {
		if (!(tasks[t] = (int*) malloc(SIZE * sizeof(int)))) {
			perror("malloc array");
			exit(EXIT_FAILURE);
		}
		if (t > 17) {
			int colNum;
			int rowNum;
			int index = t - 18;
			if (index % 3 == 0) {
				colNum = 0;
				rowNum = index;
			} else {
				colNum = (index % 3) * 3;
				rowNum = (index) - (index % 3);
			}
			int cur = 0;
			for (int i = rowNum; i < rowNum + 3; ++i) {
				for (int j = colNum; j < colNum + 3; ++j) {
					tasks[t][cur++] = s->mat[i][j];
				}
			}
			cur = 0;
		} else if (t > 8) {
			for (int i = 0; i < SIZE; i++) {
				tasks[t][i] = s->mat[i][t % SIZE];
			}
		} else {
			copySudokuRow(tasks[t], s->mat[t]);
		}
//		for (int i = 0; i < SIZE; ++i) {
//			printf("tasks[%d][%d] = %d\n", t, i, tasks[t][i]);
//		}
	}
}

void readFromStandardInput(sudoku_mat* s) {
	printf("Enter Sudoku Matrix;\n\n");
	for (int i = 0; i < SIZE; ++i) {
		printf("Enter row #%d\n", i + 1);
		for (int j = 0; j < SIZE; ++j) {
			scanf("%d ", &s->mat[i][j]);
		}
	}
}

boolean readMatrixFromFile(FILE *f, char* fileName, sudoku_mat* s) {
	int i, j;
	if ((f = fopen(fileName, "r")) == NULL) {
		printf("Cannot Open File '%s'", fileName);
		return false;
	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (!fscanf(f, "%d", &s->mat[i][j])) {
				break;
			}
		}
	}
	fclose(f);
	return true;
}

void printSudokuMatrix(sudoku_mat* s) {
	int i, j;
	printf("\n");
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			printf("%2d", s->mat[i][j]);
		}
		printf("\n");
	}
}

void copySudokuRow(int dest[SIZE], int src[SIZE]) {
	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i];
	}
}
