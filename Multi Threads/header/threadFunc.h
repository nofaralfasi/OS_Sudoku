#ifndef HEADER_THREADFUNC_H_
#define HEADER_THREADFUNC_H_

void *rowsChecker(void * argShared);
void *colsChecker(void * argShared);
void *subMatricesChecker(void * argShared);
void getOffset(int *y_offset, int *x_offset, int subMat);

#endif /* HEADER_THREADFUNC_H_ */
