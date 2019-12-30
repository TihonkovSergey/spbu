#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;

int* prod_matrix_by_rows(int** a, int* b, int m, int n, int threads){
	int* result = new int[m];
	for (int i = 0; i < m; i++)
		result[i] = 0;
#pragma omp parallel shared(a, b, result) num_threads(threads)
	{
#pragma omp for nowait
		for (int i = 0; i < m; i++){
			for (int j = 0; j < n; j++)
				result[i] += a[i][j] * b[j];
		}
	}
	return result;
}

int* prod_matrix_by_columns(int** a, int* b, int m, int n, int threads){
	int* result = new int[m];
	for (int i = 0; i < m; i++)
		result[i] = 0;
#pragma omp parallel shared(a, b, result) num_threads(threads)
	{
#pragma omp for
		for (int j = 0; j < n; j++){
			for (int i = 0; i < m; i++)
				result[i] += a[i][j] * b[j];
		}
	}
	return result;
}

int* prod_matrix_by_blocks(int** a, int* b, int m, int n, int threads){
	int* result = new int[m];
	for (int i = 0; i < m; i++)
		result[i] = 0;
#pragma omp parallel shared(a, b, result) num_threads(threads)
	{
		int s = omp_get_num_threads();
		int q = s;
		int block_h = m / s;
		int block_w = n / q;

#pragma omp for
		for (int blocks = 0; blocks < s*q; blocks++){
			int i = blocks / q;
			int j = blocks % q;
			for (int k = i*block_h; k < (i + 1) * block_h; k++)
				for (int l = j*block_w; l < (j + 1) * block_w; l++)
					result[k] += a[k][l] * b[l];
		}
	}
	return result;
}

int main() {
	// creating and opening output files
	ofstream file_rows;
	file_rows.open("output_rows.txt");

	ofstream file_columns;
	file_columns.open("output_columns.txt");

	ofstream file_blocks;
	file_blocks.open("output_blocks.txt");

	int max_n_threads = 16;
	int max_dim = 10000;

	// filling output files table borders
	for (int dim = 10; dim <= max_dim; dim *= 10){
		file_rows << dim << " ";
		file_columns << dim << " ";
		file_blocks << dim << " ";
	}
	file_rows << endl;
	file_columns << endl;
	file_blocks << endl;

	// main loop
	for (int threads_n = 1; threads_n <= max_n_threads; threads_n *= 2){
		cout << "Number of threads: " << threads_n << endl;
		file_rows << threads_n << " ";
		file_columns << threads_n << " ";
		file_blocks << threads_n << " ";
		for (int dim = 10; dim <= max_dim; dim *= 10){
			cout << "Dimension: " << dim << endl;
			int m = dim;
			int n = dim;

			// fill random
			int** a = new int*[m];
			for (int i = 0; i < m; i++){
				a[i] = new int[n];
				for (int j = 0; j < n; j++)
					a[i][j] = rand() % 100;
			}
			int* b = new int[n];
			for (int i = 0; i < n; i++)
				b[i] = rand() % 100;

			// time for rows
			auto start_rows = chrono::system_clock::now();
			int* result_rows = prod_matrix_by_rows(a, b, m, n, threads_n);
			auto end_rows = chrono::system_clock::now();

			file_rows << chrono::duration_cast<chrono::microseconds>(end_rows - start_rows).count() << " ";

			// time for columns
			auto start_columns = chrono::system_clock::now();
			int* result_columns = prod_matrix_by_columns(a, b, m, n, threads_n);
			auto end_columns = chrono::system_clock::now();

			file_columns << chrono::duration_cast<chrono::microseconds>(end_columns - start_columns).count() << " ";

			// time for blocks
			auto start_blocks = chrono::system_clock::now();
			int* result_blocks = prod_matrix_by_blocks(a, b, m, n, threads_n);
			auto end_blocks = chrono::system_clock::now();

			file_blocks << chrono::duration_cast<chrono::microseconds>(end_blocks - start_blocks).count() << " ";

			// cleaning memory
			for (int i = 0; i < m; i++)
				delete[] a[i];
			delete[] a;
			delete[] b;
			delete[] result_rows;
			delete[] result_columns;
			delete[] result_blocks;
		}
		cout << endl;
		file_rows << endl;
		file_columns << endl;
		file_blocks << endl;
	}

	// close output files
	file_rows.close();
	file_columns.close();
	file_blocks.close();

	return 0;
}