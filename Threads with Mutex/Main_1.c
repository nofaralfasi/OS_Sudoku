

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions_1.h"



int main(int argc, char **argv)
{
	FILE* f = NULL;
	Sudoku s;
	pthread_t threads[NUM_THREADS];
	thread_data threads_data[NUM_THREADS];
	int ans[NUM_THREADS] =
	{ 0 };
	int answer = 0;
	get_sudoku(argc, &s, argv, f);
	for (int t = 0; t < NUM_THREADS; ++t)
	{
		if (t > 17) // Minors
		{
			copySudokuMinors(threads_data,t,&s);

		}
		else if (t > 8) //cols
		{
			copySudokuCols(threads_data, t, &s);
		}
		else
		{
			copySudokuRow(threads_data[t].arr, s.mat[t]);

		}
		int rc = pthread_create(&threads[t], NULL, checkArr,
				(void *) &threads_data[t].arr);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for (int t = 0; t < NUM_THREADS; ++t)
	{
		pthread_join(threads[t], (void**) &ans[t]);

	}

	checkAnswer(answer, ans, argv);
}

