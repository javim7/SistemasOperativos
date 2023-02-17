#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    { // Child process
        execl("./ipc", "ipc", "15", "a", NULL);
        perror("execl");
        return 1;
    }
    else
    { // Parent process
        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        { // Child process
            usleep(5000);
            execl("./ipc", "ipc", "10", "b", NULL);
            perror("execl");
            return 1;
        }
        else
        { // Parent process
            wait(NULL);
            wait(NULL);
            return 0;
        }
    }
}
