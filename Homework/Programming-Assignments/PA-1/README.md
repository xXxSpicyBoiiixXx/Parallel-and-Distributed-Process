# Programming Assignment 1 
## CS 546 Parallel and Distributed Processing 
## Md Ali

### Problem 1
We are asked to implement a sorting algorithm for an arbitary number of processes and try scattering and gather chunks by using collective calls in MPI.
Here we utilized our own code utilizing merge sort where each process will get the exact amount to sort and then reconstruct the sorted arrays that are
then merged together and displayed on the console.

#### To compile
Simply type "make" and the program should compile 

#### Running exercise 1 
To run exercise 1, type "mpirun -n <number of processes> ./exercise-1 <array size>" 
 

### Problem 2
