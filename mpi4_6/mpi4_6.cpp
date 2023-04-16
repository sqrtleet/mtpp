#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef struct {
    int count;
    double sum;
} AvgData;

void avg_op(void* inarr, void* inoutarr, int* len, MPI_Datatype* datatype) {
    AvgData* in = (AvgData*)inarr;
    AvgData* inout = (AvgData*)inoutarr;

    for (int i = 0; i < *len; i++) {
        inout[i].count += in[i].count;
        inout[i].sum += in[i].sum;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double data = rank + 1;

    MPI_Datatype types[2] = { MPI_INT, MPI_DOUBLE };
    int block_lengths[2] = { 1, 1 };
    MPI_Aint offsets[2];
    offsets[0] = offsetof(AvgData, count);
    offsets[1] = offsetof(AvgData, sum);

    MPI_Datatype MPI_AvgData;
    MPI_Type_create_struct(2, block_lengths, offsets, types, &MPI_AvgData);
    MPI_Type_commit(&MPI_AvgData);

    AvgData input;
    input.count = 1;
    input.sum = data;

    AvgData result;
    result.count = 0;
    result.sum = 0.0;

    MPI_Op avg_operation;
    MPI_Op_create(avg_op, 1, &avg_operation);

    MPI_Reduce(&input, &result, 1, MPI_AvgData, avg_operation, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double average = result.sum / result.count;
        printf("Average: %f\n", average);
    }

    MPI_Op_free(&avg_operation);
    MPI_Type_free(&MPI_AvgData);
    MPI_Finalize();
    return 0;
}





// C:\Users\Student\Desktop\3Aproject\project-AAA\lab3_nikiforov\x64\Debug
// C:\Users\sosatb\source\repos\mtpp2\x64\Debug
// mpiexec -n 4 mpi4_6.exe