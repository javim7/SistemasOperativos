#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_ITERACIONES 10

int recursos = 10;
sem_t semaforo;
pthread_mutex_t log_mutex;

void write_log(int thread_id, const char *message)
{
    pthread_mutex_lock(&log_mutex);
    printf("%d - %s\n", thread_id, message);
    pthread_mutex_unlock(&log_mutex);
}

void *thread_function(void *thread_id)
{
    int id = *((int *)thread_id);
    int i;
    srand(time(NULL) + id);

    // Mostrar mensaje de apertura de semáforo
    write_log(id, "Semaforo abierto con exito");

    for (i = 0; i < NUM_ITERACIONES; i++)
    {
        // Mostrar número de iteración
        char iteration_message[30];
        sprintf(iteration_message, "Iniciando iteracion %d", i + 1);
        write_log(id, iteration_message);

        // Consumir un recurso
        sem_wait(&semaforo);
        recursos--;
        write_log(id, "(!) Recurso tomando");
        sem_post(&semaforo);

        // Simular actividad con el recurso consumido
        int sleep_time = rand() % 3 + 1;
        sleep(sleep_time);

        // Mostrar mensaje de recurso usado
        write_log(id, "Recurso usado");

        // Devolver el recurso
        sem_wait(&semaforo);
        recursos++;
        write_log(id, "Recurso devuelto");
        sem_post(&semaforo);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int i;

    pthread_mutex_init(&log_mutex, NULL);
    sem_init(&semaforo, 0, 1);

    write_log(0, "Creando threads");

    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i + 1;
        write_log(thread_ids[i], "Iniciando thread");
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
    }

    write_log(0, "Esperando threads");

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Devolver todos los recursos
    sem_wait(&semaforo);
    recursos = 10;
    sem_post(&semaforo);

    sem_destroy(&semaforo);
    pthread_mutex_destroy(&log_mutex);

    return 0;
}
