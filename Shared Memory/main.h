#ifndef SUDOKU_MAIN_H
#define SUDOKU_MAIN_H

#define MAT_SIZE 9
#define PROCESS_NUM 3
#define LEGAL_SEQUENCE 3
#define OFFSET 6
#define DIFF 3
#define BASE 10
#define SOLUTION_SIZE (MAT_SIZE * MAT_SIZE * sizeof(int))
#define STATUS_SIZE (PROCESS_NUM * sizeof(int))
#define MEM_LENGTH (SOLUTION_SIZE + STATUS_SIZE)

typedef struct shared {
    int solution[MAT_SIZE][MAT_SIZE];
    int status[3];
} SHARED;

void fork_and_assign(pid_t *process, SHARED *shared_struct_obj);

void init_process(int i, SHARED *shared_struct_obj);

void read_from_terminal_and_write_matrix(int matrix[MAT_SIZE][MAT_SIZE], SHARED *shared_struct_obj);

void read_from_file_and_write_matrix(int matrix[MAT_SIZE][MAT_SIZE], char *filename, SHARED *shared_struct_obj);

void check_sudoku(char *filePath, SHARED *shared_struct_obj);

void print_matrix(int matrix[MAT_SIZE][MAT_SIZE]);

int checkMatrixRows(int matrix[MAT_SIZE][MAT_SIZE]);

int checkMatrixCols(int matrix[MAT_SIZE][MAT_SIZE]);

int checkSubMatrix(int matrix[MAT_SIZE][MAT_SIZE]);

void wait_for_children();

#endif //SUDOKU_MAIN_H
