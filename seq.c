#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int matrixC[SIZE][SIZE];

int main() {
  int i, j, k;

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      matrixA[i][j] = 1;
      matrixB[i][j] = 2;
    }
  }

  clock_t start = clock();

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      matrixC[i][j] = 0.0;
      for (k = 0; k < SIZE; k++)
        matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
    }
  }

  clock_t end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;


        printf(" ____                                   _       _\n");
        printf("/ ___|  ___  __ _ _   _  ___ _ __   ___(_) __ _| |\n");
        printf("\\___ \\ / _ \\/ _` | | | |/ _ \\ '_ \\ / __| |/ _` | |\n");
        printf(" ___) |  __/ (_| | |_| |  __/ | | | (__| | (_| | |\n");
        printf("|____/ \\___|\\__, |\\__,_|\\___|_| |_|\\___|_|\\__,_|_|\n");
        printf("               |_|\n");
  printf("Tempo de execução sequencial: %f\n", time_spent);

  return 0;
}

