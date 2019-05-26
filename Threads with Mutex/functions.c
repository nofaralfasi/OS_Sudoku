#include <stdio.h>
#include <stdlib.h>
#include "functions_2.h"

void print_result(int error, char** argv) {
	if (error == 0) {
		printf("%s is legal\n", argv[1]);
	} else {
		printf("%s is not legal\n", argv[1]);
	}
}
void mutex_and_attr(pthread_mutex_t* res_mutex, pthread_attr_t* attr) {
	printf("mutex_and_attr\n");
	printf("pthread_mutex_init(res_mutex, NULL);\n");
	pthread_mutex_init(res_mutex, NULL);
	printf("pthread_attr_init(attr);\n");
	pthread_attr_init(attr);
	printf("pthread_attr_setdetachstate(&*attr, PTHREAD_CREATE_JOINABLE);\n");
	pthread_attr_setdetachstate(&*attr, PTHREAD_CREATE_JOINABLE);
}

void getSudoku(int argc, Sudoku* s, char** argv, FILE* f) {
	if (argc < 2) {
		readSudokuFromUser(&*s);
	}
	char* filename = argv[1];
	if (readSudokuFromFile(f, filename, &*s) == -1) {
		perror("read from file");
		exit(-1);
	}
}

void end_thread(pthread_mutex_t* mutex, int* number_of_running_threads,
		pthread_cond_t* tasksCond) {
	printf("end_thread\n");
	pthread_mutex_lock(mutex);
	(*number_of_running_threads)++;
	pthread_mutex_unlock(mutex);
	pthread_cond_signal(tasksCond);
}

void* threadWork(void *threadarg) {
	printf("threadWork\n");
	int my_mission;
	int* missionIndex;
	int myRes = 0;
	int** allTasks;
	int* father_res;
	int* number_of_running_threads;
	pthread_cond_t* tasksCond;
	pthread_mutex_t* mutex;
	missionIndex = ((thread_data *) threadarg)->missionIndex;
	father_res = ((thread_data *) threadarg)->my_errors;
	allTasks = ((thread_data *) threadarg)->Allmiss;
	mutex = ((thread_data *) threadarg)->mutex;
	tasksCond = ((thread_data *) threadarg)->tasksCond;
	number_of_running_threads =
			((thread_data *) threadarg)->number_of_running_threads;
	while ((my_mission = get_index_of_mission(missionIndex, mutex)) < 27) {
		if (*father_res != 0) {
			end_thread(mutex, number_of_running_threads, tasksCond);
			pthread_exit(NULL);
		}

		myRes = checkArr(allTasks[my_mission]);
		if (myRes == 0) {
			*father_res += 1;	//mutex not need
		}

	}
	end_thread(mutex, number_of_running_threads, tasksCond);
	pthread_exit(NULL);

}

int get_index_of_mission(int* missionIndex, pthread_mutex_t* mutex) {
	printf("get_index_of_mission\n");
	pthread_mutex_lock(mutex);
	(*missionIndex)++;
	pthread_mutex_unlock(mutex);
	return (*missionIndex) - 1;
}

int checkArr(int* arr) {
	printf("checkArr\n");
	int checker[SUDOKU_LENGTH] = { 0 };
	for (int i = 0; i < SUDOKU_LENGTH; ++i) {
		int tmp = arr[i];
		if (!checker[tmp - 1]) {
			checker[tmp - 1] = arr[i];
		} else {
			return 0;
		}
	}
	for (int j = 0; j < SUDOKU_LENGTH; ++j) {
//		printf("(!checker[j]); (!checker[j])) is %d,  j is %d\n", (!checker[j]), j);
		if (!checker[j]) {
			return 0;
		}
	}
	return 1;

}

void missions(int** allMiss, Sudoku* s) {
	printf("missions\n");
	for (int t = 0; t < NUM_OF_TASK; ++t) {
		if (!(allMiss[t] = (int*) malloc(SUDOKU_LENGTH * sizeof(int)))) {
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
					allMiss[t][cur++] = s->mat[i][j];
				}
			}
			cur = 0;
		} else if (t > 8) {
			for (int i = 0; i < SUDOKU_LENGTH; i++) {
				allMiss[t][i] = s->mat[i][t % SUDOKU_LENGTH];
			}
		} else {
			copySudokuRow(allMiss[t], s->mat[t]);
		}
		for (int i = 0;  i < SUDOKU_LENGTH; ++i) {
			printf("allMiss[%d][%d] = %d\n",t,i,allMiss[t][i]);

		}
		printf("-------\n");
		printf("-------------------var[%d].my_res = %d\n",t,*var[t].my_res);
	}
}

void readSudokuFromUser(Sudoku* s) {
//	printf("enter sudoku manually;\n\n");
	for (int i = 0; i < SUDOKU_LENGTH; ++i) {
		printf("row number %d\n", i + 1);
		for (int j = 0; j < SUDOKU_LENGTH; ++j) {
			scanf("%d ", &s->mat[i][j]);
		}
	}
}

int readSudokuFromFile(FILE *f, char* fileName, Sudoku* s) {
	int i, j;
	if ((f = fopen(fileName, "r")) == NULL) {
		printf("Cannot Open File '%s'", fileName);
		return 0;
	}
	for (i = 0; i < SUDOKU_LENGTH; i++) {
		for (j = 0; j < SUDOKU_LENGTH; j++) {
			if (!fscanf(f, "%d", &s->mat[i][j])) {
				break;
			}
		}
	}
	fclose(f);
	return 1;
}

void printSudoku(Sudoku* s) {
	int i, j;
	for (i = 0; i < SUDOKU_LENGTH; i++) {
		for (j = 0; j < SUDOKU_LENGTH; j++) {
			printf("%2d", s->mat[i][j]);
		}
		printf("\n");
	}
}

//void* checkArr(void *threadarg)
//{
//	int myRes = 0;
//
//	int checker[SUDOKU_LENGTH] =
//	{ 0 };
//	for (int i = 0; i < SUDOKU_LENGTH; ++i)
//	{
//		int tmp = ((threads_data *) threadarg)->arr[i];
//		if (!checker[tmp - 1])
//		{
//			checker[tmp - 1] = ((threads_data *) threadarg)->arr[i];
//		}
//		else
//		{
//			myRes = 0;
//
//			pthread_mutex_lock(((threads_data *) threadarg)->mutex);
//			*((threads_data*) threadarg)->my_res += myRes;
//			pthread_mutex_unlock(((threads_data *) threadarg)->mutex);
//			pthread_exit(((threads_data*) threadarg)->my_res);
//
//		}
//	}
//	for (int j = 0; j < SUDOKU_LENGTH; ++j)
//	{
//		if (!checker[j])
//		{
//			myRes = 0;
//			pthread_mutex_lock(((threads_data *) threadarg)->mutex);
//			*((threads_data*) threadarg)->my_res += myRes;
//			pthread_mutex_unlock(((threads_data *) threadarg)->mutex);
//			pthread_exit(((threads_data*) threadarg)->my_res);
//		}
//	}
//	myRes = 1;
//
//	pthread_mutex_lock(((threads_data *) threadarg)->mutex);
//	*((threads_data*) threadarg)->my_res += myRes;
////	printf("myres = %d\n myid = %d\nmy mutex = %p\n\n------\n", *((threads_data*) threadarg)->my_res,((threads_data*) threadarg)->tid,((threads_data*) threadarg)->mutex);
//	pthread_mutex_unlock(((threads_data *) threadarg)->mutex);
////	((thread_data*) threadarg)->my_res = 1;
//	pthread_exit(((threads_data*) threadarg)->my_res);
//
//}

void copySudokuRow(int dest[SUDOKU_LENGTH], int src[SUDOKU_LENGTH]) {

	for (int i = 0; i < SUDOKU_LENGTH; ++i) {
		dest[i] = src[i];

	}

}
