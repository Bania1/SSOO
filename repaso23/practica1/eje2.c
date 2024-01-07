#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main (int argc, char **argv[1])
{
    if (argc != 2)
    {
        printf("Error al ejecutar el fichero\n");
        exit(EXIT_SUCCESS);
    }

    int N = atoi(argv[1]);

    pid_t pid, flag;
    int status;

    pid = fork();
     
    for (int i = 0; i < N; i++)
    {
        
    }
    

    // for (int i = 0; i < N; i++)
    // {
    //     pid = fork();

    //     switch (pid)
    //     {
    //     case -1:
    //         printf("Error al crear el hijo\n");
    //         exit(EXIT_FAILURE);
    //         break;
    //     case 0: 
    //         printf("[hijo] --> pid: %ld y [padre] --> pid: %ld\n", (long int)getpid(), (long int)getppid());


    //     default:
            
    //         break;
    //     }
    // }
}