#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pid1, pid2, pid3;
  clock_t start, end;
  int i,j,k;

  start = clock();

  pid1 = fork();

  if (pid1 == 0) {
    // Proceso hijo
    pid2 = fork();

    if (pid2 == 0) {
      // Proceso nieto

      for (i = 0; i < 1000000; i++) {
        printf("Iteracion: %d\n", i);
      }


      wait(NULL);
    } else {
      // Proceso hijo
      pid3 = fork();

      if (pid3 == 0) {
        // Proceso bisnieto

        for (j = 0; j < 1000000; j++) {
            printf("Iteracion: %d\n", j);
        }

      } else {
        // Proceso hijo

        for (int k = 0; k < 1000000; k++) {
            printf("Iteracion: %d\n", k);
        }


        wait(NULL);
      }
    }
  } else {
    // Proceso padre
    wait(NULL);

    end = clock();

    double elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo total: %f segundos.\n", elapsed_time);
  }

  return 0;
}
