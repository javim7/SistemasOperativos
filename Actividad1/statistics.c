#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 5

int list[100];
int list_size;
double mean, maximum, minimum, standard_deviation, median;

pthread_mutex_t mutex_sum;

void *calcularMedia(void *arg)
{
    // Reserva memoria para almacenar el resultado
    double *result = malloc(sizeof(double));
    // Variable para almacenar la suma de los elementos de la lista
    double sum = 0.0;

    // Recorre la lista y suma sus elementos
    for (int i = 0; i < list_size; i++)
    {
        sum += list[i];
    }

    // Calcula la media de la lista
    mean = sum / list_size;

    // Almacena el resultado en la memoria reservada anteriormente
    *result = mean;

    // Finaliza el thread y devuelve el resultado
    pthread_exit(result);
}

void *calcularMaximo(void *arg)
{
    // Reserva memoria para almacenar el resultado
    double *result = malloc(sizeof(double));
    // Variable para almacenar el valor máximo de la lista
    maximum = list[0];

    // Recorre la lista desde el segundo elemento y compara con el máximo actual
    for (int i = 1; i < list_size; i++)
    {
        if (list[i] > maximum) // Si el elemento actual es mayor que el máximo actual
        {
            maximum = list[i]; // Actualiza el máximo
        }
    }

    // Almacena el resultado en la memoria reservada anteriormente
    *result = maximum;

    // Finaliza el thread y devuelve el resultado
    pthread_exit(result);
}

void *calcularMinimo(void *arg)
{
    // Reserva memoria para almacenar el resultado
    double *result = malloc(sizeof(double));

    // Variable para almacenar el valor mínimo de la lista
    minimum = list[0];

    // Recorre la lista desde el segundo elemento y compara con el mínimo actual
    for (int i = 1; i < list_size; i++)
    {
        if (list[i] < minimum) // Si el elemento actual es menor que el mínimo actual
        {
            minimum = list[i]; // Actualiza el mínimo
        }
    }

    // Almacena el resultado en la memoria reservada anteriormente
    *result = minimum;

    // Finaliza el thread y devuelve el resultado
    pthread_exit(result);
}

void *calcularDesvEst(void *arg)
{
    // Reserva memoria para almacenar el resultado
    double *result = malloc(sizeof(double));

    // Variable para almacenar la suma de los cuadrados de las diferencias de cada elemento con la media
    double sum_of_squares = 0.0;

    // Recorre la lista y suma los cuadrados de las diferencias de cada elemento con la media
    for (int i = 0; i < list_size; i++)
    {
        sum_of_squares += pow(list[i] - mean, 2);
    }

    // Calcula la desviación estándar de la lista
    standard_deviation = sqrt(sum_of_squares / list_size);

    // Almacena el resultado en la memoria reservada anteriormente
    *result = standard_deviation;

    // Finaliza el thread y devuelve el resultado
    pthread_exit(result);
}

void *calcularMediana(void *arg)
{
    // Reserva memoria para almacenar el resultado
    double *result = malloc(sizeof(double));

    // Variables para ordenar la lista
    int i, j;
    double temp;

    // Ordena la lista en orden ascendente usando el algoritmo de bubble sort
    for (i = 0; i < list_size - 1; i++)
    {
        for (j = i + 1; j < list_size; j++)
        {
            if (list[i] > list[j]) // Si el elemento actual es mayor que el siguiente elemento
            {
                // Intercambia los elementos
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }

    // Calcula la mediana de la lista
    if (list_size % 2 == 0) // Si la lista tiene un número par de elementos
    {
        median = (list[list_size / 2 - 1] + list[list_size / 2]) / 2.0; // La mediana es el promedio de los dos elementos del medio
    }
    else // Si la lista tiene un número impar de elementos
    {
        median = list[list_size / 2]; // La mediana es el elemento del medio
    }

    // Almacena el resultado en la memoria reservada anteriormente
    *result = median;

    // Finaliza el thread y devuelve el resultado
    pthread_exit(result);
}

int main(int argc, char *argv[])
{
    // Comprobar que se hayan ingresado suficientes argumentos
    if (argc < 2)
    {
        printf("Uso: %s <lista de enteros>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Inicializar hilos y mutex
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex_sum, NULL);

    // Almacenar tamaño de la lista
    list_size = argc - 1;

    // Convertir argumentos en lista de enteros
    for (int i = 1; i < argc; i++)
    {
        list[i - 1] = atoi(argv[i]);
    }

    // Crear hilos para calcular estadísticas
    pthread_create(&threads[0], NULL, calcularMedia, NULL);
    pthread_create(&threads[1], NULL, calcularMaximo, NULL);
    pthread_create(&threads[2], NULL, calcularMinimo, NULL);
    pthread_create(&threads[3], NULL, calcularDesvEst, NULL);
    pthread_create(&threads[4], NULL, calcularMediana, NULL);

    double *result;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Esperar a que los hilos terminen y obtener los resultados
        pthread_join(threads[i], (void **)&result);

        // Imprimir los resultados dependiendo del hilo
        if (i == 0)
        {
            printf("Media   : %.3lf\n", *result);
        }
        else if (i == 1)
        {
            printf("Máximo  : %.3lf\n", *result);
        }
        else if (i == 2)
        {
            printf("Mínimo  : %.3lf\n", *result);
        }
        else if (i == 3)
        {
            printf("Desv Est: %.3lf\n", *result);
        }
        else if (i == 4)
        {
            printf("Mediana : %.3lf\n", *result);
        }
        free(result);
    }

    // Liberar recursos
    pthread_mutex_destroy(&mutex_sum);
    pthread_exit(NULL);
}
