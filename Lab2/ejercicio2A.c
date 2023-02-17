#include <stdio.h>
#include <time.h>

int main() {
    clock_t start_time, end_time;
    double total_time;
    int i, j, k;

    start_time = clock();

    for(i=0; i<1000000; i++) {
        printf("Ciclo 1: %d\n", i);
    }

    for(j=0; j<1000000; j++) {
        printf("Ciclo 2: %d\n", j);
    }

    for(k=0; k<1000000; k++) {
        printf("Ciclo 3: %d\n", k);
    }

    end_time = clock();
    total_time = ((double)(end_time - start_time))/CLOCKS_PER_SEC;
    printf("Tiempo total: %f segundos\n", total_time);

    return 0;
}
