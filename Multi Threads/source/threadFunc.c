#include <stdio.h>
#include <pthread.h>
#include "../header/threadFunc.h"
#include "../header/config.h"

void *rowsChecker(void *arg) {
	SHARED *shared_data = (SHARED *) arg;
	int row = shared_data->index;
	printf("\nchecking row #%i", row);

	int result = 1;
	int check_list[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < MAT_SIZE; ++i) {
		if (shared_data->gameState_to_share->solution[row][i] < MAT_SIZE + 1
				&& shared_data->gameState_to_share->solution[row][i] > 0) {
			++check_list[shared_data->gameState_to_share->solution[row][i] - 1];
		} else {
			result = 0;
			break;
		}
	}
	for (int k = 0; k < MAT_SIZE; ++k) {
		if (check_list[k] != 1) {
			result = 0;
			break;
		}
		check_list[k] = 0;
	}
	printf("   row #%i result: %i", row, result);
	shared_data->gameState_to_share->status_rows[row] = result;

	pthread_exit(NULL);
	return NULL;
}

void *colsChecker(void *arg) {
	SHARED *shared_data = (SHARED *) arg;
	int col = shared_data->index;
	printf("\nchecking col #%i", col);

	int result = 1;
	int check_list[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < MAT_SIZE; ++i) {
		if (shared_data->gameState_to_share->solution[i][col] < MAT_SIZE + 1
				&& shared_data->gameState_to_share->solution[i][col] > 0) {
			++check_list[shared_data->gameState_to_share->solution[i][col] - 1];
		} else {
			result = 0;
			break;
		}
	}
	for (int k = 0; k < MAT_SIZE; ++k) {
		if (check_list[k] != 1) {
			result = 0;
			break;
		}
		check_list[k] = 0;
	}
	printf("   col #%i result: %i", col, result);
	shared_data->gameState_to_share->status_cols[col] = result;
	pthread_exit(NULL);
		return NULL;
}

void *subMatricesChecker(void *arg) {
	SHARED *shared_data = (SHARED *) arg;
	int subMat = shared_data->index;
	printf("\nchecking sub matrix #%i", subMat);

	int result = 1, y_offset, x_offset;
	int check_list[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	getOffset(&y_offset, &x_offset, subMat);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (shared_data->gameState_to_share->solution[i + y_offset][j
					+ x_offset] < MAT_SIZE + 1
					&& shared_data->gameState_to_share->solution[i + y_offset][j
							+ x_offset] > 0) {
				++check_list[shared_data->gameState_to_share->solution[i
						+ y_offset][j + x_offset] - 1];
			} else {
				result = 0;
				break;
			}
		}
	}
	for (int k = 0; k < MAT_SIZE; ++k) {
		if (check_list[k] != 1) {
			result = 0;
			break;
		}
	}
	printf("   subMat #%i result: %i", subMat, result);
	shared_data->gameState_to_share->status_subMat[subMat] = result;

	pthread_exit(NULL);
		return NULL;
}

void getOffset(int *y_offset, int *x_offset, int subMat) {
	if (subMat >= 0 && subMat <= 2)
		*y_offset = 0;

	if (subMat >= 3 && subMat <= 5)
		*y_offset = 3;

	if (subMat >= 6 && subMat <= 8)
		*y_offset = 6;

	if (subMat == 0 || subMat == 3 || subMat == 6)
		*x_offset = 0;

	if (subMat == 1 || subMat == 4 || subMat == 7)
		*x_offset = 3;

	if (subMat == 2 || subMat == 5 || subMat == 8)
		*x_offset = 6;
}
