/*
Javier Mombiela
Carnet: 20067
Seccion: 21

Lab 3: Sudoku Validator
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <stdbool.h>
#include <omp.h>

// Declaración del arreglo bidimensional de 9x9
int sudoku[9][9];
#define THREAD_COUNT 3

int valid = 1;
int res_check;

// Función para verificar si todos los números del 1 al 9 están en una fila dada
// en un arreglo de 9x9.
int check_rows()
{
    int i, j, k;
    omp_set_num_threads(9);
    omp_set_nested(true);
#pragma omp parallel for private(j, k, valid) schedule(dynamic)

    for (i = 0; i < 9; i++) // Recorre todas las filas del arreglo de 9x9
    {
        int nums[9] = {0}; // Arreglo auxiliar para llevar registro de los números que aparecen en la fila actual
#pragma omp parallel for private(j) schedule(dynamic)
        for (j = 0; j < 9; j++) // Recorre todas las columnas de la fila actual
        {
            k = sudoku[i][j] - 1; // Obtiene el valor de la celda actual
            if (nums[k])          // Si el número ya apareció en la fila, entonces no cumple con la regla del Sudoku
            {
                valid = 0; // Retorna 0 para indicar que la fila no cumple con la regla del Sudoku
            }
            nums[k] = 1; // Marca que el número actual ya apareció en la fila
        }
    }
    return valid; // Si todas las filas cumplen con la regla del Sudoku, entonces retorna 1
}

// Función para verificar si todos los números del 1 al 9 están en una columna dada
// en un arreglo de 9x9.
int check_columns()
{
    int i, j, k;
    omp_set_num_threads(9);
    omp_set_nested(true);
#pragma omp parallel for private(j, k, valid) schedule(dynamic)
    for (i = 0; i < 9; i++) // Recorre todas las columnas del arreglo de 9x9
    {
        printf("En la revision de columnas el siguiente es un thread en ejecucion: %ld\n", syscall(SYS_gettid)); // Imprime el ID del hilo que se está ejecutando
        int nums[9] = {0};
#pragma omp parallel for private(k, valid) schedule(dynamic)
        // Arreglo auxiliar para llevar registro de los números que aparecen en la columna actual
        for (j = 0; j < 9; j++) // Recorre todas las filas de la columna actual
        {
            k = sudoku[j][i] - 1; // Obtiene el valor de la celda actual
            if (nums[k])          // Si el número ya apareció en la columna, entonces no cumple con la regla del Sudoku
            {
                valid = 0; // Retorna 0 para indicar que la columna no cumple con la regla del Sudoku
            }
            nums[k] = 1; // Marca que el número actual ya apareció en la columna
        }
    }
    return valid; // Si todas las columnas cumplen con la regla del Sudoku, entonces retorna 1
}

// Funcion que llama a check_columns
void *check_columns2(void *arg)
{
    printf("El thread que ejecuta el metodo para ejecutar el metodo de revision de columnas es: %ld\n", syscall(SYS_gettid)); // Imprime el número de hilo
    check_columns();                                                                                                          // Llama a la función check_columns para verificar si todas las columnas cumplen con la regla del Sudoku
    pthread_exit(0);                                                                                                          // Finaliza la ejecución del hilo
}

// Función para revisar subarreglo de 3x3 dentro de un arreglo de 9x9
int check_subarray(int row, int col)
{
    int i, j, k;
    omp_set_num_threads(3);
    omp_set_nested(true);
    int nums[9] = {0}; // Inicializar un arreglo de 9 elementos en cero para llevar un registro de los números presentes en el subarreglo.
#pragma omp parallel for private(j, k, valid) schedule(dynamic)
    for (i = row; i < row + 3; i++) // Iterar por las filas del subarreglo.
    {
#pragma omp parallel for private(k, valid) schedule(dynamic)
        for (j = col; j < col + 3; j++) // Iterar por las columnas del subarreglo.
        {
            k = sudoku[i][j] - 1; // Obtener el número presente en la celda (i,j) del subarreglo y restar uno para obtener el índice correspondiente en el arreglo nums.
            if (nums[k])          // Si el número ya está presente en el arreglo nums, significa que hay una repetición en el subarreglo.
            {
                valid = 0; // Retornar 0 para indicar que la revisión del subarreglo ha fallado.
            }
            nums[k] = 1; // Marcar la posición correspondiente en el arreglo nums para indicar que el número está presente en el subarreglo.
        }
    }
    return valid; // Si la revisión del subarreglo es exitosa, retornar 1.
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(1);
    // Abrir el archivo sudoku.txt
    int fd;
    struct stat fileStat;
    char *fileContent;
    char *filePath = argv[1];
    pthread_t threads[THREAD_COUNT];

    // Abrir archivo y mapearlo a memoria
    fd = open(filePath, O_RDONLY);
    if (fd < 0)
    {
        perror("Error al abrir el archivo");
        exit(1);
    }

    if (fstat(fd, &fileStat) < 0)
    {
        perror("Error al obtener información del archivo");
        exit(1);
    }

    // ver el contenido del archivo y poderlo mapear
    fileContent = mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileContent == MAP_FAILED)
    {
        perror("Error al mapear archivo a memoria");
        exit(1);
    }

    // Copiar símbolos del archivo al arreglo bidimensional de 9x9
    int index = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            sudoku[row][col] = fileContent[index] - '0';
            index++;
        }
    }

    // Cerrar el archivo
    close(fd);

    int subArrayResult = 1;

    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            if (!check_subarray(i, j))
            {
                valid = 0;
                // printf("Subarreglo de %dx%d -> incorrecto\n", i + 1, j + 1);
            }
            else
            {
                // printf("Subarreglo de %dx%d -> correcto\n", i + 1, j + 1);
            }
        }
    }

    pid_t pid;
    pid = getpid();

    int forkId = fork();
    char pidString[10];
    sprintf(pidString, "%d", pid);

    if (forkId == 0)
    {
        execlp("ps", "ps", "-p", pidString, "-lLf", NULL);
    }
    else
    {
        if (pthread_create(&threads[0], NULL, check_columns2, NULL))
        {
            fprintf(stderr, "Error al crear el thread\n");
            return 1;
        }

        // esperar a que el thread de revisión de filas termine
        if (pthread_join(threads[0], NULL))
        {
            fprintf(stderr, "Error al esperar por el thread\n");
            return 1;
        }

        // desplegar el número de thread en ejecución
        printf("El thread en que se ejecuta en el main es: %ld\n", syscall(SYS_gettid));

        wait(NULL);

        // verfiiccar las filas del sudoku
        valid = check_rows();

        // Verificar si el sudoku es válido o no
        if (valid)
        {
            printf("\nEl sudoku SI es válido!\n");
        }
        else
        {
            printf("\nEl sudoku NO es válido!\n");
        }

        if (fork() == 0)
        {
            printf("Antes de terminar el estado de este proceso y sus threads es:\n");
            execlp("ps", "ps", "-p", pidString, "-lLf", NULL);
        }
        else
        {
            wait(NULL);
            return 0;
        }
    }

    return 0;
}
