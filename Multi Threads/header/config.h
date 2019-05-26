#ifndef HEADER_CONFIG_H_
#define HEADER_CONFIG_H_
#include <pthread.h>

#define MAT_SIZE 9
#define SOLUTION_SIZE (MAT_SIZE * MAT_SIZE * sizeof(int))

typedef struct gameState {
	int solution[MAT_SIZE][MAT_SIZE];
	int status_rows[MAT_SIZE];
	int status_cols[MAT_SIZE];
	int status_subMat[MAT_SIZE];
} GAMESTATE;

typedef struct shared {
	GAMESTATE* gameState_to_share;
	int index;
} SHARED;

#endif /* HEADER_CONFIG_H_ */
