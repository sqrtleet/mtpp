#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

void mm(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mmp(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C, int num_threads) {
    int n = A.size();
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 1000;
    vector<vector<double>> A(n, vector<double>(n, 1.0));
    vector<vector<double>> B(n, vector<double>(n, 1.0));
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    auto start = high_resolution_clock::now();
    mm(A, B, C);
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << "Elapsed time (sequential): " << elapsed.count() << " seconds" << endl;
    int num_threads[] = { 2, 4, 8 };

    for (int t : num_threads) {
        C.assign(n, vector<double>(n, 0.0));
        auto start = high_resolution_clock::now();
        mmp(A, B, C, t);
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        cout << "Elapsed time (parallel, " << t << " threads): " << elapsed.count() << " seconds" << endl;
    }

    return 0;
}
