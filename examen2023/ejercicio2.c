#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x = 1;
pthread_mutex_t semaforo;

void * h1(int *entero)
{
    int *retorna = (int *)malloc(sizeof(int));

    if (pthread_mutex_lock(&semaforo) != 0)
    {
        printf("Error en lock 1\n");
        exit(EXIT_FAILURE);
    }
    
    x += (*entero);
    printf("Soy la hebra %lu y el valor x:%d\n", pthread_self(), x);

    if (x % 2 == 0)
    {
        (*retorna) = 0; //par
    }
    else
    {
        (*retorna) = 1;
    }

    if (pthread_mutex_unlock(&semaforo) != 0)
    {
        printf("Error en unlock 1\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void *)retorna);
}

void * h2(int *entero)
{
    int *retorna = (int *)malloc(sizeof(int));
    
    if (pthread_mutex_lock(&semaforo) != 0)
    {
        printf("Error en lock 2\n");
        exit(EXIT_FAILURE);
    }
    
    x *= (*entero);
    printf("Soy la hebra %lu y el valor x:%d\n", pthread_self(), x);

    if (x % 2 == 0)
    {
        (*retorna) = 0; //par
    }
    else
    {
        (*retorna) = 1;
    }
    
    if (pthread_mutex_unlock(&semaforo) != 0)
    {
        printf("Error en unlock 2\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void *)retorna);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error en argumentos\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en init\n");
        exit(EXIT_FAILURE);
    }
    
    int entero1 = atoi(argv[1]);
    int entero2 = atoi(argv[2]);

    pthread_t hilo1, hilo2;

    if (pthread_create(&hilo1, NULL, (void *)h1, &entero1) != 0)
    {
        printf("Error create 1\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&hilo2, NULL, (void *)h2, &entero2) != 0)
    {
        printf("Error create 2\n");
        exit(EXIT_FAILURE);
    }

    int *recogido;
    int res;

    if (pthread_join(hilo1, (void**)&recogido) != 0)
    {
        printf("Error join 1\n");
        exit(EXIT_FAILURE);
    }
    res = (*recogido);
    free(recogido);

    printf("El valor recogido de la hebra %lu: %d\n", hilo1, res);

    if (pthread_join(hilo2, (void**)&recogido) != 0)
    {
        printf("Error join 2\n");
        exit(EXIT_FAILURE);
    }
    res = (*recogido);
    free(recogido);

    printf("El valor recogido de la hebra %lu: %d\n", hilo2, res);

    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en destroy\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}