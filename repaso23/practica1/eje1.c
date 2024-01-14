#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Implemente un programa que pidiendo un número de procesos totales N por línea de argumentos
cree las siguientes jerarquías de procesos en abanico (los procesos ejecutan en paralelo):

Cada proceso hijo mostrará por salida estándar un mensaje incluyendo su ID y el de su padre, y
finalizará su ejecución con código de salida 0 (recuerde que esto es simplemente hacer un exit(0),
return(0) o exit(EXIT_SUCCESS)).

El padre esperará para recoger a sus hijos a su finalización e imprimirá un mensaje indicando la
finalización de cada hijo y su status, y terminará con código 0. Utilice macros como
EXIT_FAILURE, WEXITSTATUS, etc.

*/

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error al ejecutar el fichero\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);

    pid_t pid, flag;
    int status;

    for (int i = 0; i < N; i++)
    {

        pid = fork();

        switch (pid)
        {
        case -1:
            printf("Error, hijo no creado\n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            printf("[hijo %d] --> pid: %ld y [padre] --> pid: %ld\n", i + 1, (long int)getpid(), (long int)getppid());
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
    }

    printf("[PADRE] ---> %ld\n", (long int)getpid());
    while ((flag = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("\nhijo %ld finalizado con status %d\n", (long int)flag, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("hijo %ld finalizado tras recibir una senal con status %d\n", (long int)flag, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("hijo %ld parado con status %d\n", (long int)flag, WSTOPSIG(status));
        }
        else if (WIFCONTINUED(status))
        {
            printf("hijo %ld reanudado\n", (long int)flag);
        }
    }
    if (flag == (pid_t)-1 && errno == ECHILD)
    {
        printf("Valor del errno= %d, definido como %s\n", errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}