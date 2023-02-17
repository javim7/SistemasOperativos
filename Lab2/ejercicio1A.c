#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    fork();
    fork();
    fork();

    printf("\nProcess");
    return(0);

}
