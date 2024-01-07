#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main()
{
    pid_t pid, flag;
    int status;

    // Para crear al hijo
    pid = fork();

    // Error
    if (pid == -1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    // Hijo creado
    else if (pid == 0)
    {
        printf("[HIJO] ---> %ld\n", (long int)getpid());
        exit(EXIT_SUCCESS);
    }

    // La espera del padre
    else
    {
        printf("[PADRE] ---> %ld\n", (long int)getpid());

        while((flag=wait(&status))>0)
        {
            if(WIFEXITED(status))
            {
                printf("\nhijo %ld finalizado con status %d\n",(long int)flag,WEXITSTATUS(status));
            }
            else if(WIFSIGNALED(status))
            {
                printf("hijo %ld finalizado tras recibir una senal con status %d\n",(long int)flag,WTERMSIG(status));
            }
            else if(WIFSTOPPED(status))
            {
                printf("hijo %ld parado con status %d\n",(long int)flag,WSTOPSIG(status));
            }
            else if(WIFCONTINUED(status))
            {
                printf("hijo %ld reanudado\n",(long int)flag);
            }

        }
        if(flag==(pid_t)-1 && errno==ECHILD)
        { 
            printf("Valor del errno= %d, definido como %s\n",errno,strerror(errno));
        }
        else
        {
            printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal\n");
            exit(EXIT_FAILURE);
        } 

        exit(EXIT_SUCCESS);
    } 
}