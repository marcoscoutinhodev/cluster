#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3000

int main(int argc, char* argv[]) {
    int size, rank;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_proc = N / size;

    int *A = (int*)malloc(sizeof(int) * N * N);
    int *B = (int*)malloc(sizeof(int) * N * N);
    int *C = (int*)malloc(sizeof(int) * N * N);

    if (rank == 0) {
        for (int i = 0; i < N*N; i++) {
            A[i] = 1;
            B[i] = 2;
        }
    }

    int *sub_A = (int*)malloc(sizeof(int) * elements_per_proc * N);
    int *sub_C = (int*)malloc(sizeof(int) * elements_per_proc * N);

    MPI_Scatter(A, elements_per_proc*N, MPI_INT, sub_A,
                elements_per_proc*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    for (int i = 0; i < elements_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            sub_C[i*N + j] = 0;
            for (int k = 0; k < N; k++) {
                sub_C[i*N + j] += sub_A[i*N + k] * B[k*N + j];
            }
        }
    }

    MPI_Gather(sub_C, elements_per_proc*N, MPI_INT, C,
               elements_per_proc*N, MPI_INT, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0) {
        //printf("Matriz resultante:\n");
        //for (int i = 0; i < N; i++) {
        //    for (int j = 0; j < N; j++) {
        //        printf("%d ", C[i*N + j]);
        //    }
        //    printf("\n");
        //}
        printf("Tempo de execução: %f\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}

