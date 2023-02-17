#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid = fork(); // crea un nuevo proceso

    if (pid == -1)
    { // fork() falló
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    { // proceso hijo
        printf("Soy el proceso hijo\n");
    }
    else
    { // proceso padre
        while (1)
        {
            // este ciclo se ejecutará infinitamente
        }
    }

    return 0;
}
