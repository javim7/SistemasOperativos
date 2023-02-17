#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Uso: %s <numero> <caracter>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    char c = argv[2][0];

    const int SIZE = n * sizeof(char);

    int fd = shm_open("memcompartida", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1)
    {
        perror("ftruncate");
        return 1;
    }

    char *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    int divisible;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // proceso hijo
        for (int i = 0; i < n; i++)
        {
            while (i == *ptr)
            {
                // esperar a que el padre escriba
                usleep(10);
            }

            if ((i + 1) % n == 0)
            {
                printf("Proceso hijo recibió %c en iteración %d\n", *ptr, i + 1);
            }
            ptr[i] = *ptr;
        }

        return 0;
    }

    // proceso padre
    for (int i = 0; i < n; i++)
    {
        divisible = (i + 1) % n == 0;

        if (divisible)
        {
            *ptr = c;
            printf("Proceso padre escribió %c en iteración %d\n", c, i + 1);
        }

        usleep(10);
    }

    wait(NULL);

    printf("Contenido de la memoria compartida:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%c", ptr[i]);
    }
    printf("\n");

    if (munmap(ptr, SIZE) == -1)
    {
        perror("munmap");
        return 1;
    }

    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    if (shm_unlink("memcompartida") == -1)
    {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
