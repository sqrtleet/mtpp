#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL) + rank);

    int data_elements = rank + 2;
    int* data = (int*)malloc(data_elements * sizeof(int));
    for (int i = 0; i < data_elements; ++i) {
        data[i] = rand() % 100;
    }

    int* recv_counts = (int*)malloc(size * sizeof(int));
    int* displs = (int*)malloc(size * sizeof(int));
    int total_elements = 0;
    for (int i = 0; i < size; ++i) {
        recv_counts[i] = i + 2;
        displs[i] = total_elements;
        total_elements += recv_counts[i];
    }

    int* all_data = (int*)malloc(total_elements * sizeof(int));

    MPI_Allgatherv(data, data_elements, MPI_INT, all_data, recv_counts, displs, MPI_INT, MPI_COMM_WORLD);

    printf("Process %d received data:\n", rank);
    for (int i = 0; i < total_elements; ++i) {
        if (i == displs[0]) {
            printf("Rank 0: ");
        }
        else {
            for (int j = 1; j < size; ++j) {
                if (i == displs[j]) {
                    printf("\nRank %d: ", j);
                    break;
                }
            }
        }
        printf("%d ", all_data[i]);
    }
    printf("\n");

    free(data);
    free(recv_counts);
    free(displs);
    free(all_data);

    MPI_Finalize();
    return 0;
}



// C:\Users\Student\Desktop\3Aproject\project-AAA\lab3_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi4_5.exe