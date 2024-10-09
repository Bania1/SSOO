#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Implemente un programa donde se creen dos hijos. Uno de ellos que abra la calculadora de su
distribución de Linux (busque como se llama ese ejecutable) y el otro que abra un editor de textos
con N ficheros pasados como argumentos (recuerde hacer que el padre espere a los hijos). Use para
ello la familia de funciones exec().

Un ejemplo de invocación sería:

./miPrograma gnome-calculator gedit fichero1.txt fichero2.txt ficheroN.txt

*/

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Error, no se puede ejecutar el fichero\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid, flag;
    int status;

    for (int i = 0; i < 2; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Error al crear el hijo\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (i == 0)
            {
                printf("[hijo %d] ---> pid: %ld y [padre] ---> pid: %ld\n", i + 1, (long int)getpid(), (long int)getppid());
                execlp(argv[1], argv[1], NULL);
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("[hijo %d] ---> pid: %ld y [padre] ---> pid: %ld\n", i + 1, (long int)getpid(), (long int)getppid());
                execvp(argv[2], &argv[3 - 1]);
                exit(EXIT_SUCCESS);
            }
        }
    }

    // Espera del padre
    while ((flag = wait(&status)) > 0)
    {

        if (WIFEXITED(status))
        {
            printf("hijo %ld finalizado con status %d\n", (long int)flag, WEXITSTATUS(status));
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