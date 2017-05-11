// by Serhii Savchenko

#include "stdafx.h"

using namespace std;

const int N = 300;										// Size of input vector
const int M = 500000;									// Number of iterations to do
const int T = 8;										// Number of threads

// Create Input Vector
double* generateVector(){

	double *vector = new double[N];

	for (int i = 0; i < N; ++i){
		
		double randElem = rand() % 100;					// Random value from 0 to 99
		if (randElem <= 66) {							// ~ 66% of coordinates = 0
			vector[i] = 0.0;
		}
		else {
			vector[i] = randElem;
		}
	}

	return vector;
}

// Writing to file
void writeVector(double* vector, string fileName){
	ofstream fout;
	fout.open(fileName, ios_base::trunc);				// Replace all file content
	for (int i = 0; i < N; ++i){
		fout << "[" << i << "]" << " = " << vector[i] << "; " << endl;
	}
	fout.close();
}

int main() {

	// Create new Input Vector
	double *inputVector = generateVector();
	writeVector(inputVector, "InputVector.txt");

	// Create two copies for future work	
	double *resultVector = new double[N];
	double *tempVector = new double[N];
	for (int i = 0; i < N; i++) {
		resultVector[i] = inputVector[i];
		tempVector[i] = inputVector[i];
	}	

	// Start time
	clock_t t;
	t = clock();

	for (int i = 0; i < M; i++){

		// OpenMP Parallel Part
		#pragma omp parallel for num_threads(T)	
		for (int j = 1; j < N - 1; j++) {
			if (inputVector[j] == 0) {
				tempVector[j] = (resultVector[j - 1] + resultVector[j + 1]) / 2;
			}
		}

		if(i==0){ 
			writeVector(tempVector, "FirstIteration.txt"); 
		}
		
		for (int k = 0; k < N; k++){
			resultVector[k] = tempVector[k];
		}
	}

	// End time
	t = clock() - t;
	float time = ((float)t) / CLOCKS_PER_SEC;

	// ResultVector
	writeVector(resultVector, "ResultVector.txt");

	// Results
	cout << "Vector size: " << N << "\n" <<
		"Iterations: " << M << "\n" <<
		"Threads: " << T << "\n" <<
		"Time: " << time << " seconds" << endl;

	system("Pause");
	return 0;

}