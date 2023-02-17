#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <numero> <letra>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    char x = argv[2][0];

    printf("El numero es: %d, y la letra es: %c.\n", n, x);

    return 0;
}
