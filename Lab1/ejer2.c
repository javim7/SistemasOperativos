#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 4096

int main(int argc, char* argv[]) {
    // Verificar si se han proporcionado dos argumentos
    if (argc != 3) {
        printf("Error: Numero incorrecto de parametros\n");
        return 1;
    }

    // Abrir el archivo de origen en modo lectura
    int fd_origen = open(argv[1], O_RDONLY);
    if (fd_origen == -1) {
        perror("Error al abrir origen");
        return 1;
    }

    // Abrir el archivo de destino en modo escritura
    int fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_destino == -1) {
        perror("Error al abrir destino");
        return 1;
    }

    // Copiar el contenido del archivo de origen al archivo de destino
    char buffer[BUFSIZE];
    ssize_t bytes_leidos;
    while ((bytes_leidos = read(fd_origen, buffer, BUFSIZE)) > 0) {
        write(fd_destino, buffer, bytes_leidos);
    }

    // Cerrar los archivos
    close(fd_origen);
    close(fd_destino);

    printf("El archivo se copio con exito\n");

    return 0;
}
