#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mutexMultiThreads.h"

int main(int argc, char **argv) {
	FILE* file = NULL;
	pthread_cond_t tasksCond = PTHREAD_COND_INITIALIZER;
	sudoku_mat sudoku;
	mutex_thread_info info[THRD_NUM];
	pthread_t threads[THRD_NUM];
	int error = 0, taskIndex = 0, currNumOfThreads = 0;
	pthread_attr_t attr;
	pthread_mutex_t task_mutex;
	int** allTasks;

	getSudokuMatrix(argc, &sudoku, argv, file);
	if (!(allTasks = (int**) malloc(TASKS * sizeof(int*)))) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	setTasks(allTasks, &sudoku);
	mutex_and_attr(&task_mutex, &attr);

	for (int t = 0; t < THRD_NUM; ++t) {
		info[t].tasks = allTasks;
		info[t].taskIndex = &taskIndex;
		info[t].mutex = &task_mutex;
		info[t].wrongs = &error;
		info[t].tasksCond = &tasksCond;
		info[t].currNumOfThreads = &currNumOfThreads;
		int rc = pthread_create(&threads[t], &attr, threadTask,
				(void *) &info[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_mutex_lock(&task_mutex);
	while (currNumOfThreads < THRD_NUM) {
		pthread_cond_wait(&tasksCond, &task_mutex);
	}

	pthread_mutex_unlock(&task_mutex);

	for (int t = 0; t < THRD_NUM; ++t) {
		pthread_join(threads[t], NULL);
	}

	printSudokuMatrix(&sudoku);
	printSudokuResult(error, argv);
	return 0;
}

