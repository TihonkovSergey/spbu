#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;


int** prod_matrix_by_blocks(int** a, int** b, int m, int n, int k, int threads_num){
	int** res = new int*[m];
	for (int i = 0; i < m; i++)
		res[i] = new int[k];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < k; j++)
			res[i][j] = 0;
#pragma omp parallel num_threads(threads_num)
	{
		int p = omp_get_num_threads();
		int q = p;
		int r = p;
		int a_h = m / p;
		int a_w = n / q;
		int b_w = k / r;
#pragma omp for
		for (int a_block_count = 0; a_block_count < p * q; a_block_count++){
			int i_block = a_block_count / q;
			int j_block = a_block_count % q;
			for (int t_block = 0; t_block < r; t_block++){
				for (int i = i_block * a_h; i < (i_block + 1) * a_h; i++)
					for (int j = j_block * a_w; j < (j_block + 1) * a_w; j++)
						for (int t = t_block * b_w; t < (t_block + 1) * b_w; t++)
							res[i][t] += a[i][j] * b[j][t];
			}
		}
	}
	return res;
}


int main() {
	// creating and opening output file
	ofstream file_blocks;
	file_blocks.open("output_blocks_prod.txt");

	int max_n_threads = 16;
	int max_dim = 1280;

	// filling output files table borders
	for (int dim = 20; dim <= max_dim; dim *= 4)
		file_blocks << dim << " ";
	file_blocks << endl;

	// main loop
	for (int threads = 1; threads <= max_n_threads; threads *= 2){
		cout << "Number of threads: " << threads << endl;
		file_blocks << threads << " ";
		for (int dim = 20; dim <= max_dim; dim *= 4){
			cout << "Dimension: " << dim << endl;
			int m = dim;
			int n = dim;
			int k = dim;

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

			// time for blocks
			auto start = chrono::system_clock::now();
			int** res = prod_matrix_by_blocks(a, b, m, n, k, threads);
			auto end = chrono::system_clock::now();

			file_blocks << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ";

			// cleaning memory
			for (int i = 0; i < m; i++)
				delete[] a[i];
			for (int i = 0; i < n; i++)
				delete[] b[i];
			delete[] a;
			delete[] b;
			delete[] res;
		}
		file_blocks << endl;
	}
	
	// close output file
	file_blocks.close();

	return 0;
}