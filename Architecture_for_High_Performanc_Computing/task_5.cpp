#include "omp.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;

int brute_force_search(const char* str, const char* substr, bool is_par){
	int res = -1;
#pragma omp parallel shared(str, substr) if(is_par)
	{
#pragma omp for
		for (int i = 0; i < strlen(str) - strlen(substr) + 1; i++){
			int matches_count = 0;
			for (int j = 0; j < strlen(substr); j++){
				if (str[i + j] != substr[j])
					break;
				matches_count++;
			}
			if (matches_count == strlen(substr))
				res = i;
		}
	}
	return res;
}


int main() {

	auto string = "Architecture for High-Performance Computing";
	auto substring = "High";

	auto start = chrono::system_clock::now();
	int res = brute_force_search(string, substring, false);
	auto end = chrono::system_clock::now();

	cout << "Single thread res: " << res  << endl;

	cout << "Single thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n\n";

	start = chrono::system_clock::now();
	res = brute_force_search(string, substring, true);
	end = chrono::system_clock::now();

	cout << "Multy thread res: " << res << endl;

	cout << "Multy thread time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n\n";

	return 0;
}