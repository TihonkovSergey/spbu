#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

bool is_equal(int** a, int** b, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			if (a[i][j] != b[i][j])
				return false;
	}
	return true;
}

int** prod_matrix(int** a, int** b, int m, int n, int k, bool is_par){
	int** res = new int*[m];
	for (int i = 0; i < m; i++)
		res[i] = new int[k];

#pragma omp parallel shared(a, b) if(is_par)
	{
		for (int i = 0; i < m; i++){
#pragma omp for nowait  
			for (int j = 0; j < k; j++){
				res[i][j] = 0;
				for (int s = 0; s < n; s++)
					res[i][j] += a[i][s] * b[s][j];
			}
		}
	}
	return res;
}


int main() {
	int m = 1000;
	int n = 1000;
	int k = 1000;

	// fill random
	int** a = new int*[m];
	for (int i = 0; i < m; i++){
		a[i] = new int[n];
		for (int j = 0; j < n; j++)
			a[i][j] = rand() % 100;
	}

	int** b = new int*[n];
	for (int i = 0; i < n; i++){
		b[i] = new int[k];
		for (int j = 0; j < k; j++)
			b[i][j] = rand() % 100;
	}

	// single thread
	auto start = chrono::system_clock::now();
	int** res_1 = prod_matrix(a, b, m, n, k, false);
	auto end = chrono::system_clock::now();

	// output result from single thread
	/*cout << "Single thread res:\n";
	for (int i = 0; i < m; i++){
		for (int j = 0; j < k; j++)
			cout << res_1[i][j] << " ";
		cout << endl;
	}*/

	cout << "Single thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n\n";

	// multy thread
	start = chrono::system_clock::now();
	int** res_2 = prod_matrix(a, b, m, n, k, true);
	end = chrono::system_clock::now();

	// output result from multy thread
	/*cout << "Multy thread res: \n" << endl;
	for (int i = 0; i < m; i++){
		for (int j = 0; j < k; j++)
			cout << res_2[i][j] << " ";
		cout << endl;
	}*/
	cout << "Multy thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n\n";

	// compare results 
	cout << "Is equal: " << is_equal(res_1, res_2, n, k) << "\n";

	return 0;

}