#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error en abrir el fichero\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid, flag;
    int status;

    for (int i = 0; i < 2; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Error en crear el hijo\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char *args[] = {"./a.out", argv[i + 1], NULL};
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Espera del padre
    while((flag = wait(&status)) > 0)
    {
        if(WIFEXITED(status))
        {
            printf("Hijo %ld finalizado con status %d\n", (long int)flag, WEXITSTATUS(status));
        }
        else if(WIFSIGNALED(status))
        {
            printf("Hijo %ld finalizado tras recibir una señal con status %d\n", (long int)flag, WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
            printf("Hijo %ld parado con status %d\n", (long int)flag, WSTOPSIG(status));
        }
        else if(WIFCONTINUED(status))
        {
            printf("Hijo %ld reanudado\n", (long int)flag);
        }
    }

    if(flag == (pid_t)-1 && errno == ECHILD)
    { 
        printf("Valor del errno= %d, definido como %s\n", errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocación de wait o la llamada ha sido interrumpida por una señal\n");
        exit(EXIT_FAILURE);
    } 

    exit(EXIT_SUCCESS);
}
