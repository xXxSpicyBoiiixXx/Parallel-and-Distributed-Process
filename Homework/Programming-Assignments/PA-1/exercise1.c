/*
 * FILE:
 * USAGE:
 * DESCRIPTION: 
 * OPPTIONS:
 * REQUIREMENTS: 
 * BUGS: 
 * AUTHOR: xXxSpicyBoiiixXx
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.0 
 * CREATED: 09/20/2021
 * REVISION: 
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// comparison? 
static int compare_int(const void *a, const void *b)
{
	return (*(int *) a - *(int *) b); 
}

// unfinised looking thorugh code at the momnet
// is merge sort the way to go?
// how to make this represetnated for multiple threads????  
static void merge(int *a, int numel_a, int *b, int numel_b)
{
	int *sorted = (int *) malloc((numel_a + numel_b) * sizeof *a);
	int i, a_i = 0, b_i = 0; 
	
	for (i = 0; i < (numel_a + numel_b); i++) {
		if(a_i < numel_a && b_i < numel_b) {
			if(a[a_i] < b[b_i]) { 
			sorted[i] = a[a_i];
			a_i++;		
		}
		else {
			sorted[i] = b[b_i];
			b_i++;
		}
	else { 
		if(a_i < numel_a) {
		sorted[i] = a[a_i];
		a_i++;}	
 else {
	sorted[i] = b[b_i];
	b_i++;
	}
}
}
}}


