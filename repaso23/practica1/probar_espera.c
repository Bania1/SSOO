#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Soy el padre %ld, y mi padre es %ld\n", (long int)getpid(),
           (long int)getppid());

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        perror("Error en el fork\n");
        printf("Error value = %d\n", errno);
        exit(EXIT_FAILURE);
    case 0:
        printf("Soy el hijo %ld y mi padre es %ld\n", (long int)getpid(),
               (long int)getppid());
        exit(EXIT_SUCCESS);
    }

    // Espera
    pid_t wpid;
    int status;

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
        printf("Proceso padre %ld, no hay mas hijos que esperar. valor del errno = %d, definido como %s\n ", (long int)getpid(), errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion del wait o la llamada ha sido interrumpida por una senal\n");
        exit(EXIT_FAILURE);
    }
}