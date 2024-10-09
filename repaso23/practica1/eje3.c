#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*

Modifique levemente el ejercicio 1. Implemente un programa que cree N (pedido desde línea de
argumentos) procesos utilizando fork():

 Cada proceso hijo mostrará por salida estándar un mensaje incluyendo su ID
 A continuación cada hijo esperará 10*n segundos, donde “n” va de 1 a N, de acuerdo al
orden de proceso creado, y finalizará su ejecución con código de salida 0. Recuerde que esto
es simplemente hacer un exit(0), return(0) o EXIT_SUCCESS).

 El proceso padre mostrará por salida estándar un mensaje cada vez que cree exitosamente un
hijo indicando el número de proceso creado (1..n), esperará por la finalización de todos
ellos, e imprimirá un mensaje indicando la finalización de cada hijo y su status. Terminará
con código 0.

 Verifique en otra consola los procesos creados utilizando los comandos “ps -u”, “pstree”,
“ps -a”. Deberá visualizar algo similar a la siguiente captura de pantalla. No olvide esperar a
los hijos creados fuera del bucle for principal.

¿Quién piensa que es el padre de su programa, es decir, el padre de su main() o proceso principal
que ha creado el abanico de hijos?

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

        if (pid == -1)
        {
            printf("Error, hijo no creado\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("[hijo %d] --> pid: %ld\n", i + 1, (long int)getpid());
            sleep(10 * (i + 1)); // Espera 10*n segundos
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Hijo %d creado con éxito\n", i + 1);
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
            printf("hijo %ld finalizado tras recibir una señal con status %d\n", (long int)flag, WTERMSIG(status));
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
        printf("Error en la invocación de wait o la llamada ha sido interrumpida por una señal\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
