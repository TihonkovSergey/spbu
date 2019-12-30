#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;

int* prod_matrix_by_tapes(int* a, int* b, int n, int threads) {
	int i, j, k, sum;
	int *c = new int[n*n];

#pragma omp parallel for private(j,k,sum) num_threads(threads)
	for (i = 0; i < n; i++){
		for (k = 0; k < n; k++){
			sum = 0;
			for (j = 0; j < n; j++)
				sum += a[i * n + j] * b[j * n + k];
			c[i * n + k] = sum;
		}
	}
	return c;
}


int main(){
	// creating and opening output file
	ofstream file_tapes;
	file_tapes.open("output_tapes_prod.txt");

	int max_n_threads = 16;
	int max_dim = 1280;

	// filling output files table borders
	for (int dim = 20; dim <= max_dim; dim *= 4)
		file_tapes << dim << " ";
	file_tapes << endl;

	// main loop
	for (int threads = 1; threads <= max_n_threads; threads *= 2) {
		cout << "Number of threads: " << threads << endl;
		file_tapes << threads << " ";
		for (int dim = 20; dim <= max_dim; dim *= 4) {
			cout << "Dimention: " << dim << endl;

			// fill random
			int *a = new int[dim * dim];

			int *b = new int[dim * dim];

			for (int i = 0; i < dim; i++) {
				for (int j = 0; j < dim; j++) {
					a[i*dim + j] = rand() % 100;
					b[i*dim + j] = rand() % 100;
				}
			}

			// time for tapes
			auto start = chrono::system_clock::now();
			int* res = prod_matrix_by_tapes(a, b, dim, threads);
			auto end = chrono::system_clock::now();

			file_tapes << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ";

			// cleaning memory
			delete[] a;
			delete[] b;
			delete[] res;
		}
		file_tapes << endl;
	}

	// close output file
	file_tapes.close();

	return 0;
}
