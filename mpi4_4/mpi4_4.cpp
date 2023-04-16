#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int cs = 3;
    int total_elements = size + 2;
    int* data = NULL;

    if (rank == 0) {
        data = (int*)malloc(total_elements * sizeof(int));
        for (int i = 0; i < total_elements; ++i) {
            data[i] = rand() % 100;
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    int* recv_data = (int*)malloc(cs * sizeof(int));

    int* sendcounts = NULL;
    int* displs = NULL;

    if (rank == 0) {
        sendcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; ++i) {
            sendcounts[i] = cs;
            displs[i] = i;
        }
    }

    MPI_Scatterv(data, sendcounts, displs, MPI_INT, recv_data, cs, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received: ", rank);
    for (int i = 0; i < cs; ++i) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    free(recv_data);

    if (rank == 0) {
        free(data);
        free(sendcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}



// C:\Users\Student\Desktop\3Aproject\project-AAA\lab3_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi4_4.exe