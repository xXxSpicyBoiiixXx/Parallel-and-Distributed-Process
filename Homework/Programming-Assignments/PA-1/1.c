/*
 * FILE: exercise-1.c
 * USAGE: mpirun -n <number of processes> ./exercise-1 <array size>
 * DESCRIPTION: We are asked to implement a sorting algorithm for an arbitary number of processes and try scattering and gather chunks by using collective calls in MPI.
 Here we utilized our own code utilizing merge sort where each process will get the exact amount to sort and then reconstruct the sorted arrays that are
 then merged together and displayed on the console
 * OPTIONS: -n
 * REQUIREMENTS: MPI interface
 * BUGS: N/A
 * AUTHOR: xXxSpicyBoiiixXx (Md Ali)
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.4 
 * CREATED: 09/20/2021
 * REVISION: N/A
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <limits.h> 

/*
 * Function: powerOfTwo, this function checks if the number of process are a power of two. 
 * 	     if not then an error message will appear on the console. 
 * 	     
 * Input: int id, an interger that represents the current rank of the current process 
 * 	  int numProcs, an interger with the total number of processes
 */
static void powerOfTwo(int id, int numProcs) {
	int power;

	power = (numProcs != 0) && ((numProcs & (numProcs - 1)) == 0);
	
	if (!power) {
		if (id == 0) {  
			printf("Number of processes must be power of 2 \n"); 
		} 
		
		MPI_Finalize();
		exit(-1);
	}
}


/*
 * Function: getInput, this function gets input from the user to construct an array size and then broadcasts it. 
 * 
 * Input: int argc, argument count 
 * 	  char* argv[], points to arguement vector
 * 	  int id, current rank of the current process 
 * 	  int numProcs, number of processes 
 * 	  int* arraySize, points to array size 
 */
static void getInput(int argc, char* argv[], int id, int numProcs, int* arraySize){
    if (id == 0){
        if (id % 2 != 0){
			fprintf(stderr, "usage: mpirun -n <p> %s <size of array> \n", argv[0]);
            fflush(stderr);
            *arraySize = -1;
        } else if (argc != 2){
            fprintf(stderr, "usage: mpirun -n <p> %s <size of array> \n", argv[0]);
            fflush(stderr);
            *arraySize = -1;
        } else if ((atoi(argv[1])) % numProcs != 0) {
		    fprintf(stderr, "size of array must be divisible by number of processes \n");
            fflush(stderr);
            *arraySize = -1;
		} else {
            *arraySize = atoi(argv[1]);
        }
    }

    MPI_Bcast(arraySize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (*arraySize <= 0) {
        MPI_Finalize();
        exit(-1);
    }
}

/*
 * Function: initArray, this function populates an array with random numbers
 *
 * Input: int array[], an array of integers
 * 	  int arraySize, array size 
 * 	  int id, process id
 */
static inline void initArray(int array[], int arraySize, int id) {
	for (int i = 0; i < arraySize; i++) {
		array[i] = rand() % 100; //INT_MAX
	}
}

/*
 * Function: printArray, this function prints an array with user inputted description
 *
 * Input: char arrayName[], naming array input
 *        int array[], the array itself
 *        int arraySize, the size of the array
 */
static void printArray(char arrayName[], int array[], int arraySize) {
    printf("%s", arrayName);
    for (int i = 0; i < arraySize; i++) {
        printf(" %d", array[i]);
    }
    printf("\n");
}

/*
 * Function: compare, a compariosn function used in qsort function
 *
 * Input: comparison between two ints where either -1, 0, or 1 is 
 * 	  return depending on if the first int is less than, equal,
 * 	  or greater than the second.
 */
static int compare(const void* a_p, const void* b_p) {
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else
      return 1;
}

/*
 * Function: merge, merging two halves of an array into one
 *
 * Input: int half1[], the first half of the array or part of one
 *        int half2[], the second half of the array or part of one
 *        int mergeResult[], the result of putting them together
 *         int size, the size of the array itself
 */
static int* merge(int half1[], int half2[], int mergeResult[], int size){
    int i, j, k;
    i = j = k = 0;
    
    while ((i < size) && (j < size)){
        if (half1[i] <= half2[j]){
			mergeResult[k] = half1[i];
			i++;
		} else {
			mergeResult[k] = half2[j];
			j++;
		}
			k++;
	}
	
	if (i >= size){
        while (j < size) {
			mergeResult[k] = half2[j];
			j++; k++;
		}
	}
    
	if (j >= size){
		while (i < size) {
			mergeResult[k] = half1[i];
			i++; k++;
		}
	}
	return mergeResult;
}

/*
 * Function: mergeSort, sorting algorithm for merging sorted arrays from processes until we have one array
 *
 * Input: int height, height of the tree
 *        int id, rank of the current process
 *        int localArray[], the local array a certain processes has
 *        int size, size of the localArray of the current process
 *        MPI_Comm comm, MPI communicator for communication with other processes
 *        int sharedArray[], sharedArray between all processes
 */
static int* mergeSort(int height, int id, int localArray[], int size, MPI_Comm comm, int sharedArray[]){
    int parent, rightChild, myHeight;
    int *half1, *half2, *mergeResult;

    myHeight = 0;
    qsort(localArray, size, sizeof(int), compare);
    half1 = localArray;
	
    while (myHeight < height) {
        parent = (id & (~(1 << myHeight)));

        if (parent == id) {
		    rightChild = (id | (1 << myHeight));

  		    
  		    half2 = (int*) malloc (size * sizeof(int));
  		    MPI_Recv(half2, size, MPI_INT, rightChild, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  		    
  		    mergeResult = (int*) malloc (size * 2 * sizeof(int));
  		    mergeResult = merge(half1, half2, mergeResult, size);
            half1 = mergeResult;
			size = size * 2;
			
			free(half2); 
			mergeResult = NULL;

            myHeight++;

        } else {
              MPI_Send(half1, size, MPI_INT, parent, 0, MPI_COMM_WORLD);
              if(myHeight != 0) free(half1);  
              myHeight = height;
        }
    }

    if(id == 0){
		sharedArray = half1;
	}
	return sharedArray;
}

int main(int argc, char** argv) {
    int numProcs, id, sharedArraySize, localArraySize, height;
    int *localArray, *sharedArray;
    int length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    MPI_Get_processor_name (myHostName, &length); 

    powerOfTwo(id, numProcs);

    getInput(argc, argv, id, numProcs, &sharedArraySize);

    height = log2(numProcs);

    if (id==0){
		sharedArray = (int*) malloc (sharedArraySize * sizeof(int));
		initArray(sharedArray, sharedArraySize, id);
		printArray("UNSORTED ARRAY:", sharedArray, sharedArraySize);
	}
	
    localArraySize = sharedArraySize / numProcs;
    localArray = (int*) malloc (localArraySize * sizeof(int));
    MPI_Scatter(sharedArray, localArraySize, MPI_INT, localArray,
		localArraySize, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (id == 0) {
		sharedArray = mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, sharedArray);
	}
	else {
	        mergeSort(height, id, localArray, localArraySize, MPI_COMM_WORLD, NULL);
	}

    if (id == 0) {
		printArray("FINAL SORTED ARRAY:", sharedArray, sharedArraySize);
		free(sharedArray);
	}

    free(localArray);  
    MPI_Finalize();
    return 0;
}

