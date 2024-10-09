#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Cree un programa que reciba por la linea de argumentos un número y calcule el factorial de ese
número. Compílelo y compruebe su funcionamiento.

A partir de ahí cree otro programa aparte que reciba dos números enteros como parámetros en la
linea de argumentos y cree dos procesos hijos, de manera que cada uno calcule el factorial de uno
de los números usando el ejecutable creado anteriormente (“./a.out 3 5”). use para ello la familia de
funciones exec(). En el programa que calcula el factorial ponga un sleep(1) entre los cálculos
parciales para poder observar en consola como se van ejecutando los dos procesos que se lanzarán
en paralelo.

*/

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
    while ((flag = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Hijo %ld finalizado con status %d\n", (long int)flag, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Hijo %ld finalizado tras recibir una señal con status %d\n", (long int)flag, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Hijo %ld parado con status %d\n", (long int)flag, WSTOPSIG(status));
        }
        else if (WIFCONTINUED(status))
        {
            printf("Hijo %ld reanudado\n", (long int)flag);
        }
    }

    if (flag == (pid_t)-1 && errno == ECHILD)
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
