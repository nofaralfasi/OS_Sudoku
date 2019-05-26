
#include <stdio.h>
#include <stdlib.h>
#include "functions_1.h"

void copySudokuMinors(thread_data threads_data[NUM_THREADS], int t,
		const Sudoku* s)
{
	int colNum;
	int rowNum;
	int index = t - 18;

	if (index % 3 == 0)
	{
		colNum = 0;
		rowNum = index;
	}
	else
	{
		colNum = (index % 3) * 3;
		rowNum = (index) - (index % 3);
	}
	int cur = 0;

	for (int i = rowNum; i < rowNum + 3; ++i)
	{
		for (int j = colNum; j < colNum + 3; ++j)
		{
			threads_data[t].arr[cur++] = s->mat[i][j];
		}
	}
}

void copySudokuCols(thread_data threads_data[NUM_THREADS], int t,
		const Sudoku* s)
{
	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads_data[t].arr[i] = s->mat[i][t % SUDOKU_LENGTH];
	}
}
void get_sudoku(int argc, Sudoku* s, char** argv, FILE* f)
{
	if (argc < 2)
	{
		readSudokuFromUser(&*s);
	}
	char* filename = argv[1];
	if (readSudokuFromFile(f, filename, &*s) == -1)
	{
		perror("read from file");
		exit(-1);
	}
}
void checkAnswer(int answer, int ans[NUM_THREADS], char** argv)
{
	for (int res = 0; res < NUM_THREADS; ++res)
	{
		answer += (int) ans[res];
	}
	if (answer == 27)
	{
		printf("\n%s is legal\n", argv[1]);
	}
	else
	{
		printf("\n%s is not legal\n", argv[1]);
	}
}

void readSudokuFromUser(Sudoku* s)
{
	printf("enter sudoku manually;\n\n");
	for (int i = 0; i < SUDOKU_LENGTH; ++i)
	{
		printf("row number %d\n", i + 1);
		for (int j = 0; j < SUDOKU_LENGTH; ++j)
		{
			scanf("%d ", &s->mat[i][j]);
		}
		printf("_______________");
	}
}

int readSudokuFromFile(FILE *f, char* fileName, Sudoku* s)
{
	int i, j;
	if ((f = fopen(fileName, "r")) == NULL)
	{
		printf("Cannot Open File '%s'", fileName);
		return false;
	}
	for (i = 0; i < SUDOKU_LENGTH; i++)
	{
		for (j = 0; j < SUDOKU_LENGTH; j++)
		{
			if (!fscanf(f, "%d", &s->mat[i][j]))
			{
				break;
			}
		}
	}
	fclose(f);
	return true;
}

void printSudoku(Sudoku* s)
{
	int i, j;
	for (i = 0; i < SUDOKU_LENGTH; i++)
	{
		for (j = 0; j < SUDOKU_LENGTH; j++)
		{
			printf("%2d", s->mat[i][j]);
		}
		printf("\n");
	}
}

void* checkArr(void *threadarg)
{
	int arr[SUDOKU_LENGTH];

	thread_data* my_data;
	my_data = (thread_data*) threadarg;
	int checker[SUDOKU_LENGTH] =
	{ 0 };
	copySudokuRow(arr, my_data->arr);
	for (int i = 0; i < SUDOKU_LENGTH; ++i)
	{
		int tmp = my_data->arr[i];
		if (!checker[tmp - 1])
		{
			checker[tmp - 1] = my_data->arr[i];
		}
		else
		{
			my_data->res = 0;
			((thread_data*) threadarg)->res = 0;
			pthread_exit(((thread_data*) threadarg)->res);
		}
	}
	for (int j = 0; j < SUDOKU_LENGTH; ++j)
	{
		if (!checker[j])
		{
			my_data->res = 0;
			((thread_data*) threadarg)->res = 0;
			pthread_exit(((thread_data*) threadarg)->res);
		}
	}
	my_data->res = 1;
	((thread_data*) threadarg)->res = 1;
	pthread_exit(((thread_data*) threadarg)->res);

}

void copySudokuRow(int dest[SUDOKU_LENGTH], int src[SUDOKU_LENGTH])
{
	for (int i = 0; i < SUDOKU_LENGTH; ++i)
	{
		dest[i] = src[i];
	}
}
