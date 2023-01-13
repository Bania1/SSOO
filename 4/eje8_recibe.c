#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void bunda(int signal)
{
    if (signal==SIGUSR1)
    {
        printf("HE RECIBIDO LA SENAL SIGUSR1 CON CODIGO %d\n", signal);
    }
    else if (signal==SIGKILL)
    {
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    signal(SIGUSR1, &bunda);
    signal(SIGKILL, &bunda);

    printf("Mi senal SIGUSR1 tiene pid %ld\n", (long int)getpid());

    while(1);

    exit(EXIT_SUCCESS);
}