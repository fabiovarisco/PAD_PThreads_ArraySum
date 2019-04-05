
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <math.h>

using namespace std;
using namespace std::chrono;


static int N_THREADS[] = { 2, 4, 8, 16 }, N_TOTALS[] = { 100, 1000, 10000, 100000 };

int n = 100;
int *V1, *V2, *V3;


struct vParams {
	int s;
	int n;
};

void *sumVectors(void *args) {
	vParams* p = (vParams*)args;
	for (int i = p->s; i < p->s + p->n; i++) {
		V3[i] = V1[i] * V2[i] + 20;
		V3[i] = V3[i] + V2[i] * V1[i] / 100;
		V3[i] = sqrt(V3[i]);
	}
	pthread_exit(NULL);
	return NULL;
}

void generateVector(int n, int* vector) {
	for (int i = 0; i < n; i++) {
		vector[i] = i;
	}
}

void testSumVectors(int nThreads, int nTotal) {
	int n = nTotal / nThreads;

	V1 = (int*)malloc(sizeof(int) * nTotal);
	V2 = (int*)malloc(sizeof(int) * nTotal);
	V3 = (int*)malloc(sizeof(int) * nTotal);

	generateVector(nTotal, V1);
	generateVector(nTotal, V2);

	pthread_t* tIDs = (pthread_t*)malloc(sizeof(pthread_t) * nThreads);
	int lastIndex = 0;

	for (int i = 0; i < nThreads; i++) {
		vParams *args = (vParams *)malloc(sizeof(vParams));
		args->s = lastIndex;
		args->n = n;
		if (i == nThreads - 1)
			args->n = nTotal - lastIndex;

		pthread_create(&tIDs[i], NULL, sumVectors, (void *)args);
		lastIndex += n;
	}

	for (int i = 0; i < nThreads; i++) {
		pthread_join(tIDs[i], NULL);
	}

	/*for (int i = 0; i < nTotal; i++) {
		std::cout << V3[i] << ", ";
	}*/

}

/*
void *test(void* args) {
	cout << "\n\ntest\n\n";
	return 0;
}
*/

int main() {
	cout << "hello";

	/*pthread_t tid;
	pthread_create(&tid, NULL, test, NULL);
	pthread_join(tid, NULL); */

	int n_thread_tests = sizeof(N_THREADS) / sizeof(int);
	int n_totals_tests = sizeof(N_TOTALS) / sizeof(int);

	for (int i = 0; i < n_thread_tests; i++) {
		cout << "\n\n------------------------\n"
			 << "No. Threads: " << N_THREADS[i] << " \n";
		for (int j = 0; j < n_totals_tests; j++) {
			

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			testSumVectors(N_THREADS[i], N_TOTALS[j]);
			high_resolution_clock::time_point t2 = high_resolution_clock::now();

			auto duration = duration_cast<microseconds>(t2 - t1).count();
			cout << "Array length: " << N_TOTALS[j] << ". Took: " << duration << " microseconds.\n";
		}

	}

//	testSumVectors(2, 100);

	system("pause");
}