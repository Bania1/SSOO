#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Realice el mismo ejercicio que en el apartado anterior, pero esta vez atendiendo al esquema que
muestra la siguiente figura. Los hijos deben mostrar también su ID y el de su padre, al igual que en
el ejercicio 1:

*/

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error al ejecutar el fichero\n");
        exit(EXIT_SUCCESS);
    }

    int N = atoi(argv[1]);

    pid_t pid, flag;
    int status;

    for (int i = 0; i < N; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }

        else if (pid == 0)
        {
            printf("[hijo %d] --> %ld, [padre %d] --> %ld\n", i + 1, (long int)getpid(), i, (long int)getppid());
        }

        else
        {
            // espera
            // printf("[PADRE] ---> %ld\n", (long int)getpid());
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
    }
    exit(EXIT_SUCCESS);
}