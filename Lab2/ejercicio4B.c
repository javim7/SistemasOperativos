#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork(); // crea un nuevo proceso

    if (pid == -1)
    { // fork() falló
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    { // proceso hijo
        int i;
        for (i = 1; i <= 4000000; i++)
        {
            printf("%d\n", i);
        }
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
