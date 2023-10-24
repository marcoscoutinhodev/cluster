
#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define SIZE 600

int main(int argc, char **argv) {
  int matrixA[SIZE][SIZE];
  int matrixB[SIZE][SIZE];
  int matrixC[SIZE][SIZE];
  int localA[SIZE][SIZE];
  int localC[SIZE][SIZE];

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  //clock_t start = clock();

  if (world_rank == 0) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        matrixA[i][j] = 1;
        matrixB[i][j] = 2;
      }
    }
  }

  // Broadcast matrixB to all processes
  MPI_Bcast(&matrixB, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);

  // Scatter rows of matrixA to all processes
  int sendcount = SIZE*SIZE/world_size;
  MPI_Scatter(&matrixA, sendcount, MPI_INT, &localA,
              sendcount, MPI_INT, 0, MPI_COMM_WORLD);

  clock_t start = clock();
  // Each process performs its computation
  for (int i = 0; i < SIZE/world_size; i++) {
    for (int j = 0; j < SIZE; j++) {
      localC[i][j] = 0;
      for (int k = 0; k < SIZE; k++)
        localC[i][j] += localA[i][k] * matrixB[k][j];
    }
  }

  clock_t end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("%s: %f\n", processor_name, time_spent);

  // Gather all partial results to the final result matrix
  MPI_Gather(&localC, sendcount, MPI_INT, &matrixC,
            sendcount, MPI_INT, 0, MPI_COMM_WORLD);

  /*if (world_rank == 0) {

    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        printf("%d ", matrixC[i][j]);
      }

      printf("\n");
    }

  }*/

  // Finalize the MPI environment.
  MPI_Finalize();

  return 0;
}

