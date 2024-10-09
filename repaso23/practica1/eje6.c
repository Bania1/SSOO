#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Use el ejercicio 1a) y cree una variable global de tipo entero inicializada a 0. Haga que cada hijo
aumente en uno el valor de esa variable global y que el padre imprima el resultado final. ¿Qué
ocurre?

Correcto, su valor no se modifica porque los hijos son procesos nuevos que no comparten
memoria. Para ello, y concretamente en sistemas basados en POSIX, se utilizan métodos de
intercomunicación de procesos como son memoria compartida y semáforos, los cuales se estudiarán
en otra práctica de la asignatura.

*/

int global = 0;

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
            global++;
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
    }

    printf("[PADRE] ---> %ld y variable global: %d\n", (long int)getpid(), global);
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