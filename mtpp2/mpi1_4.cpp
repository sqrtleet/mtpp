#include <iostream>
#include <ctime>
#include <cstdlib>
#include "mpi.h"

using namespace std;

double f(double x) {
    return 4 / (1 + x * x);
}

double left(double a, double b, double n, double start_offset, double n_local) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (double i = start_offset; i < start_offset + n_local; i++) {
        sum += h * f(a + i * h);
    }
    return sum;
}

int main(int argc, char** args) {
    int size, rank;
    double a = 0;
    double b = 1;
    int n_global = 1000;
    int n_local;
    double s = 0;
    double sum = 0;
    double pi;

    MPI_Status status;
    MPI_Init(&argc, &args);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    if (rank == 0) {
        int remaining = n_global;
        for (int i = 1; i < size; ++i) {
            int limit = remaining - 10 * (size - i);
            n_local = 10 + rand() % (limit - 10 + 1);
            remaining -= n_local;
            MPI_Send(&n_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        n_local = remaining;
    }
    else {
        MPI_Recv(&n_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int start_offset;
    MPI_Scan(&n_local, &start_offset, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    start_offset -= n_local;

    s = left(a, b, n_global, start_offset, n_local);
    printf("rank = %d, n_local = %d, sum = %f\n", rank, n_local, s);

    MPI_Reduce(&s, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("total_sum = %f\n", sum);
        pi = sum;
        printf("pi = %f\n", pi);
    }
    MPI_Finalize();
}


// C:\Users\Student\Desktop\3Aproject\project-AAA\lab2_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi1_4.exe
