/*
Javier Mombiela
Carnet: 20067
Seccion: 11

funciones.c: Programa que permite consumir una cantidad dada de recursos en lugar de solo uno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Definición de constantes
#define NUM_THREADS 10
#define NUM_ITERATIONS 3

pthread_mutex_t mutex;
int available_resources = 10;

// Función que disminuye la cantidad de recursos disponibles
int decrease_count(int count)
{
    pthread_mutex_lock(&mutex);
    printf("Mutex adquirido, entrando al monitor\n");

    // Verifica si hay suficientes recursos disponibles
    if (available_resources < count)
    {
        printf("Recursos insuficientes disponibles\n");
        pthread_mutex_unlock(&mutex);
        return -1; // Retorna un código de error
    }
    else
    {
        printf("Recursos suficientes disponibles, consumiendo...\n");
        available_resources -= count;
        printf("Recursos actuales: %d\n", available_resources);
        printf("Terminando decrease_count\n");
        pthread_mutex_unlock(&mutex);
        printf("Mutex liberado, saliendo del monitor\n");
        return 0; // Retorna éxito
    }
}

// Función que aumenta la cantidad de recursos disponibles
int increase_count(int count)
{
    pthread_mutex_lock(&mutex);
    printf("Iniciando increase_count\n");
    available_resources += count; // Aumenta la cantidad de recursos disponibles
    printf("Recursos agregados: %d\n", count);
    printf("Recursos actuales: %d\n", available_resources);
    pthread_mutex_unlock(&mutex);
    printf("Terminando increase_count\n");
    return 0; // Retorna éxito
}

// Función que ejecuta cada thread
void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;

    printf("Iniciando thread %d\n", thread_id);

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        printf("Iniciando iteracion %d\n", i + 1);
        int count = rand() % 3 + 1;
        printf("Se consumiran %d recursos\n", count);

        printf("Iniciando decrease_count\n");
        while (decrease_count(count) != 0)
        {
            printf("WOW! Recursos insuficientes disponibles\n");
            sleep(1); // Espera un segundo y vuelve a intentar
        }
        printf("%d - (!) Recurso tomado\n", thread_id);

        sleep(1);

        printf("Recurso usado\n");

        printf("%d - Recurso devuelto\n", thread_id);
        increase_count(count); // Devuelve los recursos
    }

    pthread_exit(NULL);
}

// Función principal
int main()
{

    printf("Iniciando programa\n");
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int i;

    printf("Creando threads\n");
    pthread_mutex_init(&mutex, NULL);

    // Crea los threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i + 1;
        printf("Iniciando thread %d\n", thread_ids[i]);
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
        sleep(1); // Espera un segundo antes de crear el siguiente thread
    }

    // Espera a que los threads terminen
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destruye el mutex
    pthread_mutex_destroy(&mutex);

    // Termina el programa
    printf("Fertig!\n");

    return 0;
}
