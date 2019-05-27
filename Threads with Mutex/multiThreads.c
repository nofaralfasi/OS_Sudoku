#include <stdio.h>
#include <stdlib.h>
#include "multiThreads.h"

void copySudokuMinors(thread_info threads_info[THRD_NUM], int t,
		const sudoku_mat* s) {
	int col, row;
	int index = t - 18;
	if (index % 3 == 0) {
		col = 0;
		row = index;
	} else {
		col = (index % 3) * 3;
		row = (index) - (index % 3);
	}
	int cur = 0;
	for (int i = row; i < row + 3; ++i) {
		for (int j = col; j < col + 3; ++j) {
			threads_info[t].arr[cur++] = s->mat[i][j];
		}
	}
}

void copySudokuCols(thread_info threads_info[THRD_NUM], int t,
		const sudoku_mat* s) {
	for (int i = 0; i < THRD_NUM; i++) {
		threads_info[t].arr[i] = s->mat[i][t % SIZE];
	}
}

void getSudokuMatrix(int argc, sudoku_mat* s, char** argv, FILE* f){
	if (argc < 2) {
		readFromStandardInput(&*s);
	}
	char* filename = argv[1];
	if (readMatrixFromFile(f, filename, &*s) == -1) {
		perror("read from file");
		exit(-1);
	}
}
void checkSudokuResult(int answer, int ans[THRD_NUM], char** argv) {
	for (int res = 0; res < THRD_NUM; ++res) {
		answer += (int) ans[res];
	}
	if (answer == 27) {
		printf("\n%s is legal\n", argv[1]);
	} else {
		printf("\n%s is not legal\n", argv[1]);
	}
}

void readFromStandardInput(sudoku_mat* s) {
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
		printf("/ncannot Open File '%s'", fileName);
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
			printf("%d ", s->mat[i][j]);
		}
		printf("\n");
	}
}

void* checkArr(void *arg) {
	int arr[SIZE];
	thread_info* info;
	info = (thread_info*) arg;
	int check[SIZE] = { 0 };
	copySudokuRow(arr, info->arr);
	for (int i = 0; i < SIZE; ++i) {
		int tmp = info->arr[i];
		if (!check[tmp - 1]) {
			check[tmp - 1] = info->arr[i];
		} else {
			info->res = 0;
			((thread_info*) arg)->res = 0;
			pthread_exit(((thread_info*) arg)->res);
		}
	}
	for (int j = 0; j < SIZE; ++j) {
		if (!check[j]) {
			info->res = 0;
			((thread_info*) arg)->res = 0;
			pthread_exit(((thread_info*) arg)->res);
		}
	}
	info->res = 1;
	((thread_info*) arg)->res = 1;
	pthread_exit(((thread_info*) arg)->res);

}

void copySudokuRow(int dest[SIZE], int src[SIZE]) {
	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i];
	}
}
