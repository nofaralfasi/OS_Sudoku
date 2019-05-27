#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "multiThreads.h"

int main(int argc, char **argv) {
	FILE* f = NULL;
	sudoku_mat s;
	pthread_t threads[THRD_NUM];
	thread_info threads_info[THRD_NUM];
	int ans[THRD_NUM] = { 0 };
	int answer = 0;
	getSudokuMatrix(argc, &s, argv, f);
	for (int t = 0; t < THRD_NUM; ++t) {
		if (t > 17)
				{
			copySudokuMinors(threads_info, t, &s);

		} else if (t > SIZE - 1) {
			copySudokuCols(threads_info, t, &s);
		} else {
			copySudokuRow(threads_info[t].arr, s.mat[t]);

		}
		int rc = pthread_create(&threads[t], NULL, checkArr,
				(void *) &threads_info[t].arr);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for (int t = 0; t < THRD_NUM; ++t) {
		pthread_join(threads[t], (void**) &ans[t]);

	}

	printSudokuMatrix(&s);
	checkSudokuResult(answer, ans, argv);
}

