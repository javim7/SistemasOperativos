#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 10
#define NUM_ITERATIONS 3

pthread_mutex_t mutex;
int available_resources = 10;

/* Decrease available resources by count resources */
/* Return 0 if sufficient resources available, */
/* otherwise return -1 */
int decrease_count(int count)
{
    pthread_mutex_lock(&mutex);
    printf("Mutex adquirido, entrando al monitor\n");
    if (available_resources < count)
    {
        printf("Recursos insuficientes disponibles\n");
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    else
    {
        printf("Recursos suficientes disponibles, consumiendo...\n");
        available_resources -= count;
        printf("Recursos actuales: %d\n", available_resources);
        pthread_mutex_unlock(&mutex);
        printf("Mutex liberado, saliendo del monitor\n");
        return 0;
    }
}

/* Increase available resources by count */
int increase_count(int count)
{
    pthread_mutex_lock(&mutex);
    available_resources += count;
    printf("Recursos agregados: %d\n", count);
    printf("Recursos actuales: %d\n", available_resources);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;

    printf("Iniciando thread %d\n", thread_id);

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        printf("Iniciando iteracion %d\n", i + 1);
        int count = rand() % 3 + 1; // Random number between 1 and 3
        printf("Se consumiran %d recursos\n", count);

        printf("Iniciando decrease_count\n");
        while (decrease_count(count) != 0)
        {
            printf("WOW! Recursos insuficientes disponibles\n");
            sleep(1);
        }
        printf("%d - (!) Recurso tomado\n", thread_id);

        // Simulate doing some work with the resources
        sleep(1);

        printf("Recurso usado\n");

        printf("%d - Recurso devuelto\n", thread_id);
        increase_count(count);
    }

    pthread_exit(NULL);
}

int main()
{
    printf("Iniciando programa\n");
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int i;

    printf("Creando threads\n");
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i + 1;
        printf("Iniciando thread %d\n", thread_ids[i]);
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
        sleep(1);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Terminando programa\n");

    return 0;
}
