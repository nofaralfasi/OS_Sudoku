#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS      4
#define ARRAYSIZE   100
#define CHUNK   ARRAYSIZE / NTHREADS

typedef struct {
	int id;					// thread id
	double *arr; 			// shared array
	double *sum;			//shared answer for sum of all elemnts
	pthread_mutex_t *mutex;	// mutex to synchronize a write to sum
} ThreadParam;

void *do_work(void *param) {
	int i, start, end, tid;
	double mysum = 0.0;
	double *arr, *sum;
	pthread_mutex_t *mutex;

	// Thread ID, array, mutex and global sum
	tid = ((ThreadParam *) param)->id;
	arr = ((ThreadParam *) param)->arr;
	mutex = ((ThreadParam *) param)->mutex;
	sum = ((ThreadParam *) param)->sum;

	/* Initialize my chunk of the shared array and calculate a local sum of my chunk*/
	start = tid * CHUNK;
	end = start + CHUNK;
	printf("Thread %d doing chunk from %d to %d\n", tid, start, end - 1);
	for (i = start; i < end; i++) {
		arr[i] = i * 1.0;
		mysum = mysum + arr[i];
	}

	/* Lock the mutex and update the global sum, then unlock and exit */
	pthread_mutex_lock(mutex);
	*sum = *sum + mysum;
	pthread_mutex_unlock(mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i;
	ThreadParam params[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	double sum = 0.0, array[ARRAYSIZE];
	pthread_mutex_t sum_mutex;

	/* Pthreads setup: initialize mutex and explicitly create threads in a
	 joinable state (for portability).  Pass each thread its loop offset */
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < NTHREADS; i++) {
		params[i].arr = array;
		params[i].id = i;
		params[i].sum = &sum;
		params[i].mutex = &sum_mutex;
		pthread_create(&threads[i], &attr, do_work, (void *) &params[i]);
	}

	/* Wait for all threads to complete then print global sum */
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("Done. Sum calculated by %d threads = %e \n", NTHREADS, sum);

	sum = 0.0;
	for (i = 0; i < ARRAYSIZE; i++) {
		array[i] = i * 1.0;
		sum = sum + array[i];
	}
	printf("Check Sum = %e\n", sum);

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit(NULL);
}
