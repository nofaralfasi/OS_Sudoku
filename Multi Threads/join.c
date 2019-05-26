#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// in this example the main thread creates 2 threads, and each thread is responsible for increasing all numbers on one
// half of an array by 1 (+1).
// the main thread waits the other 2 threads finish (by using join), and then display the result.

typedef struct {
	int *ar;
	int n;
} Subarray;

#define SIZE  400

void *incrementor(void *arg) {
	int i;

	// Increment each member of the array
	for (i = 0; i < ((Subarray *) arg)->n; i++)
		((Subarray *) arg)->ar[i]++;

	pthread_exit(NULL);
	return NULL;
}

int main(void) {
	int ar[SIZE] = { 0 };
	pthread_t th1, th2;
	Subarray sb1, sb2;

	// Create a thread that will increment the First part of array ar
	sb1.ar = &ar[0];
	sb1.n = SIZE / 2;
	(void) pthread_create(&th1, NULL, incrementor, (void *) &sb1);

	// Create another thread that will increment the Second part of array ar
	sb2.ar = &ar[SIZE / 2];
	sb2.n = SIZE / 2;
	(void) pthread_create(&th2, NULL, incrementor, (void *) &sb2);

	// Wait till both threads will finish their job
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	// Display result of common work of two threads
	for (int i = 0; i < SIZE; i++)
		printf("%d \n", ar[i]);

	return 0;
}
