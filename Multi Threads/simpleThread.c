#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	5

void *printHello(void *param) {

	// access to parameters passed to this thread
	int tid = *(int *) param;

	// Perform an action
	printf("Hello World! It's me, thread #%d!\n", tid);

	// Clean exit
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int tid[NUM_THREADS];
	int rc;
	int t;

	// Launch threads and pass the thread id as a parameter
	for (t = 0; t < NUM_THREADS; t++) {
		printf("In main: creating thread %d\n", t);
		tid[t] = t;
		rc = pthread_create(&threads[t], NULL, printHello, (void *) (&tid[t]));
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);
}
