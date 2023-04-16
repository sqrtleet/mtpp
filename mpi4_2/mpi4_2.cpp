#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

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

    int* gathered_data = NULL;
    int* recvs = NULL;
    int* displs = NULL;
    int total_elements = (size * (size + 1)) / 2 + size;
    

    if (rank == 0) {
        gathered_data = (int*)malloc(total_elements * sizeof(int));
        recvs = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; ++i) {
            recvs[i] = i + 2;
            if (i > 0) {
                displs[i] = displs[i - 1] + recvs[i - 1];
            }
            else {
                displs[i] = 0;
            }
        }
    }

    MPI_Gatherv(data, data_elements, MPI_INT, gathered_data, recvs, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered data:\n");
        int index = 0;
        for (int i = 0; i < size; ++i) {
            printf("Rank %d: ", i);
            for (int j = 0; j < recvs[i]; ++j) {
                printf("%d ", gathered_data[index++]);
            }
            printf("\n");
        }

        qsort(gathered_data, total_elements, sizeof(int), cmp);

        printf("Sorted: ");
        for (int i = 0; i < total_elements; ++i) {
            printf("%d ", gathered_data[i]);
        }
        printf("\n");

        free(gathered_data);
        free(recvs);
        free(displs);
    }

    free(data);
    MPI_Finalize();
    return 0;
}


// C:\Users\Student\Desktop\3Aproject\project-AAA\lab3_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi4_2.exe