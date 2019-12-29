#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

int get_min(int* a, int m){
	int min = a[0];
	for (int i = 0; i < m; i++){
		if (a[i] < min)
			min = a[i];
	}
	return min;
}


int max_search(int** a, unsigned int n, unsigned int m, bool is_par){
	int* mins = new int[n];

#pragma omp parallel if(is_par)
	{
#pragma omp for
		for (int i = 0; i < n; i++)
			mins[i] = get_min(a[i], m);
	}

	int max = mins[0];
#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		if (mins[i] > max)
			max = mins[i];
	}
	return max;
}



int main() {
	int n = 10000; // 1000
	int m = 10000; //1000
	int** a = new int*[n];
	
	// fill random 
	for (int i = 0; i < n; i++){
		a[i] = new int[m];
		for (int j = 0; j < m; j++)
			a[i][j] = rand() % 100000;
	}
	
	// single thread 
	auto start = chrono::system_clock::now();
	int res = max_search(a, n, m, false);
	auto end = chrono::system_clock::now();

	cout << "Single thread res: " << res << "\n";
	cout << "Single thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n\n";
	
	// multy thread
	start = chrono::system_clock::now();
	res = max_search(a, n, m, true);
	end = chrono::system_clock::now();

	cout << "Multy thread res: " << res << "\n";
	cout << "Multy thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";
	
	// cleaning memory
	for(int i = 0; i < n; i++)
		delete a[i];
	
	return 0;
}