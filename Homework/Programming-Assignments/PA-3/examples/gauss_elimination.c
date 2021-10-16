#include <stdio.h>
#include <math.h>

/*******
 Function that performs Gauss-Elimination on a Tridiagonal system and returns the Upper triangular matrix and solution of equations:
There are two options to do this in C.
1. Pass the augmented matrix (a) as the parameter, and calculate and store the upperTriangular(Gauss-Eliminated Matrix) in it.
2. Use malloc and make the function of pointer type and return the pointer.
This program uses the first option.
********/
void gaussEliminationTri(int m, int n, double a[m][n], double x[n-1]){
    int i,j,k;
    for(i=0;i<m-1;i++){
        /*//Partial Pivoting
        for(k=i+1;k<m;k++){
            //If diagonal element(absolute vallue) is smaller than any of the terms below it
            if(fabs(a[i][i])<fabs(a[k][i])){
                //Swap the rows
                for(j=i-1;j<=i+1;j++){               
                    double temp;
                    temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
                double temp;
                temp=a[i][n-1];
                a[i][n-1]=a[k][n-1];
                a[k][n-1]=temp;
            }
        }*/
        //Begin Gauss Elimination
        for(k=i+1;k<m;k++){
            double  term=a[k][i]/ a[i][i];
            for(j=i-1;j<=i+1;j++){
                a[k][j]=a[k][j]-term*a[i][j];
            }
            a[k][n-1]=a[k][n-1]-term*a[i][n-1];
        }
         
    }
    //Begin Back-substitution
    for(i=m-1;i>=0;i--){
        x[i]=a[i][n-1];
        j=i+1;
        x[i]=x[i]-a[i][j]*x[j];
        x[i]=x[i]/a[i][i];
    }
             
}
/*******
Function that reads the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void readMatrix(int m, int n, double matrix[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            scanf("%lf",&matrix[i][j]);
        }
    } 
}
/*******
Function that prints the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void printMatrix(int m, int n, double matrix[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%lf\t",matrix[i][j]);
        }
        printf("\n");
    } 
}
/*******
Function that copies the elements of a matrix to another matrix
Parameters: rows(m),columns(n),matrix1[m][n] , matrix2[m][n]
*******/
void copyMatrix(int m, int n, double matrix1[m][n], double matrix2[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            matrix2[i][j]=matrix1[i][j];
        }
    } 
}
 
int main(){
    int m,n,i,j;
    printf("Enter the size of the augmeted matrix:\nNo. of rows (m)\n");
    scanf("%d",&m);
    printf("No.of columns (n)\n");
    scanf("%d",&n);
    //Declare a matrix to store the user given matrix
    double a[m][n];
    //Declare another matrix to store the resultant matrix obtained after Gauss Elimination
    double U[m][n];
    //Declare an array to store the solution of equations
    double x[m];
    printf("\nEnter the elements of matrix:\n");
    readMatrix(m,n,a);
    copyMatrix(m,n,a,U);
    //Perform Gauss Elimination 
    gaussEliminationTri(m,n,U,x);
    printf("\nThe Upper Triangular matrix after Gauss Eliminiation is:\n\n");
    printMatrix(m,n,U);
    printf("\nThe solution of linear equations is:\n\n");
    for(i=0;i<n-1;i++){
        printf("x[%d]=\t%lf\n",i+1,x[i]);
    }
