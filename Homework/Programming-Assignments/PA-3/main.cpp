#include <iostream>
#include <iomanip> 
#include <cmath>
#include <fstream> 
#include <vector>  
#include <string>
#include <sstream> 
#include <iterator> 
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>  
#include <mpi.h> 

//#include "mpi.h" <--- C implementation/

using namespace std;

typedef vector<string> LINE; 

void transpose_matrix(float *matrix, float transposed_matrix, int size) { 
	
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			transposed_matrix[i * size + j] = matrix[j * size + i]; 
		}
	}
}

void zero_filling(double* matrix, int size) {
	
	for(int i = 0; i < size; i++) { 
		for(int j = 0; j < size; j++) {
			matrix[i * size + j] = 0; 
		}
	}
}

void parallel_solve(double* a, double* b, double* c, double* d, int size, int process_num, int num_of_processes) {
	
	int rows_per_thread = size / num_of_processes; 
	
	// Being able to send each of these smaller matrixes to others 
	double* A_matrix = new double[size * size]; 
	double lower = new double[size * size]; 
	double upper = new double[size * size]; 

	double lower_calc = new double[size * rows_per_thread];
	double upper_calc = new double[size * rows_per_thread];

	double upper_transposed = new double[size * size]; 

	if(process_num == 0) {
		
		A_matrix = a; 
		zero_filling(upper_transposed, size); 
	}

	zero_filling(lower, size);
	zero_filling(upper, size); 

	MPI_Bcast(A_matrix, size * size, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

	
	
}


// The wiki example cpp code is below.

// Only use in step 2 and 4 of the PPT decomposition, this will also broadcast to certain nodes
/*void solve(double* a, double* b, double* c, double* d, int n) { 
	
	n--; 
	
	c[0] /= b[0];
	d[0] /= b[0]; 

	for(int i = 1; i < n; i++) { 
		
		c[i] /= b[i] - a[i] * c[i-1]; 
		d[i] = (d[i] - a[i] * d[i-1]) / (b[i] - a[i] * c[i-1]); 
	}

	d[n] = (d[n] - a[n] * d[n-1]) / (b[n] - a[n] * c[n-1]); 

	for(int i = n; i-- > 0;) { 
		d[i] -= c[i] * d[i+1]; 
	} 
	
}*/

int main() { 
	
	vector<double> a, b, c, d;
	
	int n = 4096; 
 	
	ifstream inputFileA("a.csv");
	ifstream inputFileB("b.csv"); 
	ifstream inputFileC("c.csv");
	ifstream inputFileD("d.csv");
	
	string line; 
	if(!inputFileA.good() && !inputFileB.good() && inputFileC.good() && inputFileD.good()) { 
		cout << "Error opening files" << endl; 
		return 1;
} 

	while(getline(inputFileA, line)) { 
		int itmp = 0; 
		stringstream ss(line); 

		while(ss >> itmp) { 
			string stmp; 
			a.push_back(itmp); 
			getline(ss, stmp);
		}
	}
	
	while(getline(inputFileB, line)) {
		int itmp = 0; 
		stringstream ss(line); 
		
		while(ss >> itmp) { 
			string stmp;
			b.push_back(itmp); 
			getline(ss, stmp);
		}
	}

	while(getline(inputFileC, line)) { 
		int itmp = 0;
		stringstream ss(line); 
		
		while(ss >> itmp) { 
			string stmp;
			c.push_back(itmp);
			getline(ss, stmp);
		}
	}

	while(getline(inputFileD, line)) { 
		int itmp = 0;
		stringstream ss(line);
		
		while(ss >> itmp) { 
			string stmp;
			d.push_back(itmp);
			getline(ss, stmp);
		}
	}

/*
copy(a.begin(), a.end(), ostream_iterator<double>(cout));
copy(b.begin(), b.end(), ostream_iterator<double>()); 
copy(c.begin(), c.end(), ostream_iterator<double>()); 
copy(d.begin(), d.end(), ostream_iterator<double>()); */

double a_arr[a.size()];
double b_arr[b.size()]; 
double c_arr[c.size()];
double d_arr[d.size()];

copy(a.begin(), a.end(), a_arr); 
copy(b.begin(), b.end(), b_arr); 
copy(c.begin(), c.end(), c_arr); 
copy(d.begin(), d.end(), d_arr);

solve(a_arr, b_arr, c_arr, d_arr, n); 


for(int i = 0; i < n; i++) { 
	cout << d_arr[i] << endl;
}
cout << endl << "n = " << n << endl << "n is not changed!"; 


// Testing the size of input data below
//cout << a.size() << endl << b.size() << endl << c.size() << endl << d.size() << endl;;
	
	/*if(inputFileA.good() && inputFileB.good() && inputFileC.good() && inputFileD.good()) { 
		int current_number = 0; 
		string line; 
		int pos; 
		while(getline(inputFileA, line)) { 
			a.push_back(line); 
		}

		while(inputFileB >> current_number) {
			b.push_back(current_number);
		} 

		while(inputFileC >> current_number) { 
			c.push_back(current_number);
		}

		while(inputFileD >> current_number) { 
			d.push_back(current_number);
		}

	inputFileA.close(); 
	inputFileB.close();
	inputFileC.close();
	inputFileD.close();
	cout << "The numbers are: ";
	
	for(int count = 0; count < a.size(); count++) { 
		cout << a[count] << " "; 
	}

	for(int count = 0; count < b.size(); count++) { 
		cout << b[count] << " "; 
	}

	for(int count = 0; count < c.size(); count++) { 
		cout << c[count] << " "; 
	}
		
	for(int count = 0; count < d.size(); count++) { 
		cout << d[count] << " "; 
	}

	cout << endl; 

	cout << a.size() << endl << b.size() << endl << c.size() << endl << d.size(); 
	} 
	
	else{ 
		cout << "ERROR!"; 
		exit(0); 
	}

*/ 
/*	
	int  n = 4;
	double a[4] = { 0, -1, -1, -1 };
	double b[4] = { 4,  4,  4,  4 };
	double c[4] = {-1, -1, -1,  0 };
	double d[4] = { 5,  5, 10, 23 };
	// results    { 2,  3,  5, 7  }
	solve(a,b,c,d,n);
	for (int i = 0; i < n; i++) {
		cout << d[i] << endl;
	}
	cout << endl << "n= " << n << endl << "n is not changed hooray !!";
*/
	return 0;
}
