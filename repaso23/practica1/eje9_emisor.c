#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int main (int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Error, pon el pid\n");
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);

    kill(pid, SIGUSR1);
    
    sleep(1);

    kill(pid, SIGKILL);

    sleep(1);

    exit(EXIT_SUCCESS);
}