#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions_2.h"

int main(int argc, char **argv) {
	FILE* f = NULL;
	Sudoku s;
	thread_data data[NUM_THREADS];
	pthread_t threads[NUM_THREADS];
	int error = 0, missionIndex = 0, num_of_runnig_threads = 0;
	pthread_attr_t attr;
	pthread_mutex_t mission_mutex;
	pthread_cond_t tasksCond = PTHREAD_COND_INITIALIZER;
	int** allMiss;

	getSudoku(argc, &s, argv, f);
	if (!(allMiss = (int**) malloc(NUM_OF_TASK * sizeof(int*)))) {
		perror("malloc array");
		exit(EXIT_FAILURE);
	}
	missions(allMiss, &s);

	mutex_and_attr(&mission_mutex, &attr);

	for (int t = 0; t < NUM_THREADS; ++t) {
		data[t].Allmiss = allMiss;
		data[t].missionIndex = &missionIndex;
		data[t].mutex = &mission_mutex;
		data[t].my_errors = &error;
		data[t].tasksCond = &tasksCond;
		data[t].number_of_running_threads = &num_of_runnig_threads;
		int rc = pthread_create(&threads[t], &attr, threadWork,
				(void *) &data[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);

		}

	}
	pthread_mutex_lock(&mission_mutex);
	while (num_of_runnig_threads < NUM_THREADS) {
		printf("pthread_cond_wait(&tasksCond, &mission_mutex); num_of_runnig_threads is %d\n", num_of_runnig_threads);
		pthread_cond_wait(&tasksCond, &mission_mutex);
	}
	pthread_mutex_unlock(&mission_mutex);

	for (int t = 0; t < NUM_THREADS; ++t) {
		printf("pthread_join(threads[t], NULL); t is %d\n", t);
		pthread_join(threads[t], NULL);

	}
	printSudoku(&s);
	print_result(error, argv);
	return 0;
}

