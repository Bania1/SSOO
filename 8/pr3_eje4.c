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

sem_t a,b;

void *escribirA()
{
    for (int i = 0; i < 5; i++)
    {
        if (sem_wait(&a) != 0)
        {
            printf("Error sem wait a\n");
            exit(EXIT_FAILURE);
        }
        
        printf("A");
        fflush(NULL);
        
        if (sem_post(&b) != 0)
        {
            printf("Error sem post b\n");
            exit(EXIT_FAILURE);
        }    
    }
    pthread_exit(NULL);
}

void *escribirB()
{
    for (int i = 0; i < 5; i++)
    {
        if (sem_wait(&b) != 0)
        {
            printf("Error sem wait b\n");
            exit(EXIT_FAILURE);
        }
        
        printf("B");
        fflush(NULL);
        
        if (sem_post(&a) != 0)
        {
            printf("Error sem post a\n");
            exit(EXIT_FAILURE);
        }    
    }
    pthread_exit(NULL);
}

int main()
{
    if (sem_init(&b, 0, 1) != 0)
    {
        printf("Error en iniciar sem b\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&a, 0, 0) != 0)
    {
        printf("Error en iniciar sem a\n");
        exit(EXIT_FAILURE);
    }

    pthread_t id_b;
    pthread_t id_a;

    if (pthread_create(&id_b, NULL, (void *)&escribirB, NULL) != 0)
    {
        printf("Error en pthread create b\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&id_a, NULL, (void *)&escribirA, NULL) != 0)
    {
        printf("Error en pthread create a\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(id_b, NULL) != 0)
    {
        printf("Error en pthread join b\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(id_a, NULL) != 0)
    {
        printf("Error en pthread join a\n");
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&b) != 0)
    {
        printf("Error en destruir sem b\n");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&a) != 0)
    {
        printf("Error en destruir sem a\n");
        exit(EXIT_FAILURE);
    }

    printf("\n");

    exit(EXIT_SUCCESS);
}