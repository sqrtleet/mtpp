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
    
    int data[5] = { rand() % 100, rand() % 100, rand() % 100, rand() % 100, rand() % 100 };

    int* gathered_data = NULL;

    if (rank == 0) {
        gathered_data = (int*)malloc(size * 5 * sizeof(int));
    }

    MPI_Gather(data, 5, MPI_INT, gathered_data, 5, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered data:\n");
        for (int i = 0; i < size * 5; ++i) {
            if (i % 5 == 0) {
                printf("Rank %d: ", i / 5);
            }
            printf("%d ", gathered_data[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }

        qsort(gathered_data, size * 5, sizeof(int), cmp);

        printf("Sorted: ");
        for (int i = 0; i < size * 5; ++i) {
            printf("%d ", gathered_data[i]);
        }
        printf("\n");

        free(gathered_data);
    }

    MPI_Finalize();
    return 0;
}


// C:\Users\Student\Desktop\3Aproject\project-AAA\lab3_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi4_1.exe