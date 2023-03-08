/*
Javier Mombiela
Carnet: 20067
Seccion: 21

Laboratorio 3: SudokuValidator.c
*/

// importacion de librerias
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Declaración del arreglo bidimensional de 9x9
int sudoku[9][9];

/* Función para verificar que todos los números del 1 al 9 están en una fila del sudoku */
int validate_row(int row)
{
    int digits[10] = {0}; // Arreglo para contar los dígitos del 1 al 9

    for (int col = 0; col < 9; col++)
    {
        int digit = sudoku[row][col];
        if (digit < 1 || digit > 9 || digits[digit] > 0)
        {
            return 0; // Si el dígito no es válido o ya fue encontrado antes, la fila no es válida
        }
        digits[digit]++; // Incrementar el contador de dígitos
    }

    return 1; // Si se revisan todos los dígitos de la fila y todos son válidos, la fila es válida
}

/* Función para verificar que todos los números del 1 al 9 están en una columna del sudoku */
int validate_col(int col)
{
    int digits[10] = {0}; // Arreglo para contar los dígitos del 1 al 9

    for (int row = 0; row < 9; row++)
    {
        int digit = sudoku[row][col];
        if (digit < 1 || digit > 9 || digits[digit] > 0)
        {
            return 0; // Si el dígito no es válido o ya fue encontrado antes, la columna no es válida
        }
        digits[digit]++; // Incrementar el contador de dígitos
    }

    return 1; // Si se revisan todos los dígitos de la columna y todos son válidos, la columna es válida
}

/* Función para verificar que todos los números del 1 al 9 están en un subarreglo de 3x3 del sudoku */
int validate_subgrid(int row, int col)
{
    int digits[10] = {0}; // Arreglo para contar los dígitos del 1 al 9

    for (int i = row; i < row + 3; i++)
    {
        for (int j = col; j < col + 3; j++)
        {
            int digit = sudoku[i][j];
            if (digit < 1 || digit > 9 || digits[digit] > 0)
            {
                return 0; // Si el dígito no es válido o ya fue encontrado antes, el subarreglo no es válido
            }
            digits[digit]++; // Incrementar el contador de dígitos
        }
    }

    return 1; // Si se revisan todos los dígitos del subarreglo y todos son válidos, el subarreglo es válido
}

int main(int argc, char *argv[])
{
    int fd;
    struct stat fileStat;
    char *fileContent;
    char *filePath = argv[1];

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

    // Imprimir el arreglo bidimensional de 9x9
    printf("\nArreglo bidimensional de 9x9:\n");
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            printf("%d ", sudoku[row][col]);
        }
        printf("\n");
    }
    printf("\n");

    // Revisar subarreglos de 3x3
    int valid = 1;
    printf("Validación de subarreglos de 3x3:\n");
    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            if (!validate_subgrid(i, j))
            {
                valid = 0;
                printf("Subarreglo de %dx%d -> incorrecto\n", i + 1, j + 1);
            }
            else
            {
                printf("Subarreglo de %dx%d -> correcto\n", i + 1, j + 1);
            }
        }
    }

    munmap(fileContent, fileStat.st_size);
    close(fd);

    // Verificar si el sudoku es válido o no
    if (valid)
    {
        printf("\nEl sudoku SI es válido.\n");
    }
    else
    {
        printf("\nEl sudoku NO es válido.\n");
    }

    return 0;
}
