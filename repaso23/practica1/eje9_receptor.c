#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void tratarSenal(int sig)
{
    printf("He recibido la senal SIGUSR1\n");
}

int main ()
{
    signal(SIGUSR1, tratarSenal);
    printf("PID: %ld\n", (long int)getpid());

    while(1);

    exit(EXIT_SUCCESS);
}