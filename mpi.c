#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 700

int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int matrixC[SIZE][SIZE];

int main(int argc, char *argv[])
{
  int i, j, k;
  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
  {
    for (i = 0; i < SIZE; i++)
    {
      for (j = 0; j < SIZE; j++)
      {
        matrixA[i][j] = 1;
        matrixB[i][j] = 2;
      }
    }
  }

  clock_t start = clock();
  MPI_Bcast(matrixA, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrixB, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);


  for (i = rank; i < SIZE; i += size)
  {
    for (j = 0; j < SIZE; j++)
    {
      matrixC[i][j] = 0.0;
      for (k = 0; k < SIZE; k++)
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
    }
  }

  if (rank == 0)
  {
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo de execução paralelo: %f\n", time_spent);
  }

  MPI_Finalize();

  return 0;
}
