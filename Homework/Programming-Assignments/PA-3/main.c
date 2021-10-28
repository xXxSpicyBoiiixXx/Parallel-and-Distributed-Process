#include <iostream>
#include <fstream> 
#include <vector>  

using namespace std;


// Only use in step 2 and 4 of the PPT decomposition
void solve(double* a, double* b, double* c, double* d, int n) { 
	
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
	
}

int main() { 
	
	vector<double> a, b, c, d;
 
	ifstream inputFileA("a.csv");
	ifstream inputFileB("b.csv"); 
	ifstream inputFileC("c.csv");
	ifstream inputFileD("d.csv"); 
	
	if(inputFileA.good() && inputFileB.good() && inputFileC.good() && inputFileD.good()) { 
		int current_number = 0; 
		
		while(inputFileA >> current_number) { 
			a.push_back(current_number); 
		} 

	inputFileA.close(); 

	cout << "The numbers are: ";
	
	for(int count = 0; count < a.size(); count++) { 
		cout << numbers[count] << " "; 
	}

	cout << endl; 
	} 

	else{ 
		cout << "ERROR!"; 
		_exit(0); 
	}

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
