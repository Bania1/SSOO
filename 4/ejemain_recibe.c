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
        printf("\nHe recibido la señal SIGUSR1 con codigo %d\n", signal);
        
    }
    else if (signal==SIGINT)
    {
        printf("\nHe recibido la señal ctrl+c con codigo %d\n", signal);
        
    }
    else if (signal==SIGKILL)
    {
        printf("\nHe recibido la señal SIGKILL con codigo %d\n", signal);
        exit(EXIT_SUCCESS);
    }
    
}

int main()
{
    signal(SIGUSR1, &bunda);
    signal(SIGINT, &bunda);
    signal(SIGKILL, &bunda);

    printf("Mi pid es %ld\n", (long int)getpid());

    while(1);

    exit(EXIT_SUCCESS);
}
