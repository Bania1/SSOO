#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

/*

Cree dos hilos de manera que uno ejecute escribirA() y el otro escribirB(). Implemente un programa
que introduzca los semáforos oportunos para que la salida sea BABABABABA. No olvide indicar
los valores iniciales de los semáforos que utilice.

*/

#define NUM_VECES 5

sem_t a;
sem_t b;

void *escribirA()
{
    for (int i = 0; i < NUM_VECES; i++)
    {
        if (sem_wait(&a) != 0)
        {
            printf("Error en wait a\n");
            exit(EXIT_FAILURE);
        }

        printf("A");
        fflush(NULL);

        if (sem_post(&b) != 0)
        {
            printf("Error en post b\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

void *escribirB()
{
    for (int i = 0; i < NUM_VECES; i++)
    {
        if (sem_wait(&b) != 0)
        {
            printf("Error en wait b\n");
            exit(EXIT_FAILURE);
        }

        printf("B");
        fflush(NULL);

        if (sem_post(&a) != 0)
        {
            printf("Error en post a\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

int main()
{
    if (sem_init(&a, 0, 0) != 0)
    {
        printf("Error en init a\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&b, 0, 1) != 0)
    {
        printf("Error en init a\n");
        exit(EXIT_FAILURE);
    }

    pthread_t id_b;
    pthread_t id_a;

    if (pthread_create(&id_b, NULL, (void *)escribirB, NULL) != 0)
    {
        printf("Error en create b\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&id_a, NULL, (void *)escribirA, NULL) != 0)
    {
        printf("Error en create a\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(id_b, NULL) != 0)
    {
        printf("Error en join b\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(id_a, NULL) != 0)
    {
        printf("Error en join a\n");
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&b) != 0)
    {
        printf("Error en destroy b\n");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&a) != 0)
    {
        printf("Error en destroy a\n");
        exit(EXIT_FAILURE);
    }

    printf("\n");

    exit(EXIT_SUCCESS);
}