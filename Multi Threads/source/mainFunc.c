#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "../header/mainFunc.h"
#include "../header/config.h"
#include "../header/threadFunc.h"

void settingUpThreads(pthread_t thread_rows[MAT_SIZE], pthread_t thread_cols[MAT_SIZE],
                     pthread_t thread_subMat[MAT_SIZE], GAMESTATE *shared_gameState) {
    int i;
    for (i = 0; i < MAT_SIZE; i++) {
        SHARED gameState_to_share = {shared_gameState, i};

    	// Create a thread that will check matrix's rows for the sudoku solution
        (void) pthread_create(&thread_rows[i], NULL, rowsChecker, (void *) &gameState_to_share);

    	// Wait till all threads will finish their job
        pthread_join (thread_rows[i], NULL);
    }

    for (i = 0; i < MAT_SIZE; i++){
        SHARED gameState_to_share = {shared_gameState, i};
        (void) pthread_create(&thread_cols[i], NULL, colsChecker, (void *) &gameState_to_share);
        pthread_join (thread_cols[i], NULL);
    }

    for (i = 0; i < MAT_SIZE; i++){
        SHARED gameState_to_share = {shared_gameState, i};
        (void) pthread_create(&thread_subMat[i], NULL, subMatricesChecker, (void *) &gameState_to_share);
        pthread_join (thread_subMat[i], NULL);
    }

}

void readMatrixFromFile(int matrix[MAT_SIZE][MAT_SIZE], char *filename, GAMESTATE *shared_gameState){
    int i, j;
    char c;

    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
        fprintf(stderr, "File is missing.");

    for (i = 0; i < MAT_SIZE; ++i) {
        for (j = 0; j < MAT_SIZE; ++j) {
            fscanf(file, "%d%c", &matrix[i][j], &c);
        }
    }

    printSudokuMatrix(matrix);
    memcpy(shared_gameState->solution, matrix, SOLUTION_SIZE);

    fclose(file);
}

void readFromStandardInput(int matrix[MAT_SIZE][MAT_SIZE], GAMESTATE *shared_gameState){
    int i, j;
    char c;

    for(i = 0; i < MAT_SIZE; ++i) {
        printf("\n**Enter row number %d:\n", i+1);

        for(j = 0; j < MAT_SIZE; ++j) {
            scanf("%d%c", &(matrix[i][j]), &c);
        }
    }

    printSudokuMatrix(matrix);
    memcpy(shared_gameState->solution, matrix, SOLUTION_SIZE);
}

void sudokuChecker(GAMESTATE *shared_gameState) {
    int final_result = 0, i;
    for(i = 0; i < MAT_SIZE; ++i) {
        final_result += shared_gameState->status_rows[i];
        final_result += shared_gameState->status_cols[i];
        final_result += shared_gameState->status_subMat[i];
    }

    if (final_result == 27) {
        printf("\nsolution is legal\n");
    } else {
        printf("\nsolution is not legal\n");
    }
}

void printSudokuMatrix(int matrix[MAT_SIZE][MAT_SIZE]) {
    printf("\nYour matrix is:\n");
    for (int i = 0; i < MAT_SIZE; i++) {
        for (int j = 0; j < MAT_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

