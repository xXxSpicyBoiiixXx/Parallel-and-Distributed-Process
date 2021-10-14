#ifndef SEQUENTIAL_H_ 
#define SEQUENTIAL_H_

int checkError1(double **A, long n);
int checkError2(double **A, long n); 
int checkSum(double **A, long size, long version); 

double **getMatrix(long size, long version); 
double **make2dmatrix(long n); 

void decomposeSerial(double **A, long n); 
void initializeVersion1(double **A, long n); 
void initializeVersion2(double **A, long n); 
void free2dmatrix(double **A, long n); 
void printMatrix(double **A, long n);

#endif 
