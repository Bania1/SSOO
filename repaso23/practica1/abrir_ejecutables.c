/*

Implemente un programa donde se creen dos hijos. Uno de ellos que abra la calculadora de su
distribución de Linux (busque como se llama ese ejecutable) y el otro que abra un editor de textos
con N ficheros pasados como argumentos (recuerde hacer que el padre espere a los hijos). Use para
ello la familia de funciones exec().

Un ejemplo de invocación sería:

./miPrograma gnome-calculator gedit fichero1.txt fichero2.txt ficheroN.txt

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        perror("Error al abrir programa\n");
        printf("Variable del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }

    pid_t pid, wpid;
    int status;

    for (int i = 0; i < 2; i++)
    {
        pid = fork();

        switch (pid)
        {
        case -1:
            perror("Error en pid\n");
            printf("Valor del errno %d\n");
            exit(EXIT_FAILURE);
        case 0:
            if (i == 0)
            {
                printf("Soy el hijo %ld, y mi padre es %ld, ejecuto calculadora\n", (long int)getpid(), (long int)getppid());
                printf("execlp(argv[1], argv[1], NULL);\n\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("Soy el hijo %ld, y mi padre es %ld, ejecuto gedit\n", (long int)getpid(), (long int)getppid());
                printf("execvp(argv[2], &argv[3-1]);\n\n");
                exit(EXIT_SUCCESS);
            }
        }
    }

    while ((wpid = waitpid(-1, &status, WUNTRACED | WCONTINUED)) >= 0)
    {
        if (WIFEXITED(status))
        {
            printf("child exited, status=%d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("child killed (signal %d)\n", WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("child stopped (signal %d)\n", WSTOPSIG(status));

#ifdef WIFCONTINUED /* Not all implementations support this */
        }
        else if (WIFCONTINUED(status))
        {
            printf("child continued\n");
#endif
        }
        else
        { /* Non-standard case -- may never happen */
            printf("Unexpected status (0x%x)\n", status);
        }
    }

    if (wpid == (pid_t)-1 && errno == ECHILD)
    {
        printf("padre %ld, valor del errno %d, identificado como %s\n", (long int)getpid(), errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion del wait\n");
        exit(EXIT_FAILURE);
    }
}
