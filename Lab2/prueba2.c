#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHMSZ 27

int main(int argc, char **argv)
{
    int shmid;
    key_t key;
    char *shm, *s;

    // crear una clave para la memoria compartida
    key = ftok(".", 's');
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    // crear o unirse a la memoria compartida
    shmid = shmget(key, SHMSZ, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0)
    {
        if (errno == EEXIST)
        {
            printf("El espacio de memoria compartida ya fue abierto por la otra instancia.\n");

            // obtener el file descriptor de la memoria compartida de la otra instancia
            shmid = shmget(key, SHMSZ, 0666);
            if (shmid < 0)
            {
                perror("shmget");
                exit(1);
            }

            // adjuntar la memoria compartida
            shm = shmat(shmid, NULL, 0);
            if (shm == (char *)-1)
            {
                perror("shmat");
                exit(1);
            }

            // imprimir el file descriptor de la memoria compartida
            printf("El file descriptor de la memoria compartida es: %d\n", shmid);

            // separar la memoria compartida
            shmdt(shm);
            exit(0);
        }
        else
        {
            perror("shmget");
            exit(1);
        }
    }

    // adjuntar la memoria compartida
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // escribir en la memoria compartida
    s = shm;
    for (char *c = "Hello, world!"; *c != '\0'; c++)
    {
        *s++ = *c;
    }
    *s = '\0';

    // imprimir el file descriptor de la memoria compartida
    printf("El file descriptor de la memoria compartida es: %d\n", shmid);

    // separar la memoria compartida
    shmdt(shm);

    return 0;
}
