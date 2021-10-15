#ifndef OPENMP_H_
#define OPENMP_H_

void decomposeOpenMP(double **A, long n); 

int checkError1(double **A, long n); 
int checkError2(double **A, long n); 
int checkSum(double **A, long size, long loop); 

double **getMatrix(long size, long loop); 
double **make2dmatrix(long n);

void initializeLoop1(double **A, long n); 
void initializeLoop2(double **A, long n); 
void printMatrix(double **A, long n); 
void free2dmatrix(double **A, long n);


#endif 
