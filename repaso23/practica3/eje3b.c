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

Haga lo mismo pero con una sección crítica de 3 elementos, y habrá 3 productores y 3
consumidores, que podrán producir-consumir 1 elemento cada uno.

*/

#define BUFFER_SIZE 3
#define NUM_ITEMS 1
#define NUM_PROD 3
#define NUM_CONS 3

int buffer[BUFFER_SIZE] = {0};

int indice_prod = 0;
int indice_cons = 0;

sem_t s;
sem_t pr;
sem_t cn;

void *productor(int *num_producidoV)
{
    int num_producido = (*num_producidoV);
    if (sem_wait(&pr) != 0)
    {
        perror("Error en el wait pr\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_wait(&s) != 0)
    {
        perror("Error en el wait s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }

    buffer[indice_prod] = num_producido;
    indice_prod = (indice_prod + 1) % BUFFER_SIZE;

    printf("-------> [Proveedor %d] ha producido el item %d\n", num_producido, buffer[indice_prod]);

    if (sem_post(&s) != 0)
    {
        perror("Error en el post s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_post(&cn) != 0)
    {
        perror("Error en el post cn\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    pthread_exit((void *)NULL);
}

void *consumidor(int *num_consumidoV)
{
    int num_consumido = (*num_consumidoV);
    if (sem_wait(&cn) != 0)
    {
        perror("Error en el wait cn\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_wait(&s) != 0)
    {
        perror("Error en el wait s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }

    int item = buffer[indice_cons];
    indice_cons = (indice_cons + 1) % BUFFER_SIZE;

    printf("---> [Consumidor %d] ha consumido el item %d\n\n", num_consumido, item);

    if (sem_post(&s) != 0)
    {
        perror("Error en el post s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_post(&pr) != 0)
    {
        perror("Error en el post pr\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    pthread_exit((void *)NULL);
}

int main()
{
    if (sem_init(&s, 0, 1) != 0)
    {
        perror("Error al iniciar s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_init(&pr, 0, BUFFER_SIZE) != 0)
    {
        perror("Error al iniciar pr\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_init(&cn, 0, 0) != 0)
    {
        perror("Error al iniciar cn\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }

    pthread_t idhilos_productores[NUM_PROD];
    pthread_t idhilos_consumidores[NUM_CONS];

    int num_producido[NUM_PROD];
    int num_consumido[NUM_CONS];

    for (int i = 0; i < NUM_PROD; i++)
    {
        num_producido[i] = i;
        if (pthread_create(&idhilos_productores[i], NULL, (void *)productor, &num_producido) != 0)
        {
            perror("Error en el create pr\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_CONS; i++)
    {
        num_consumido[i] = i;
        if (pthread_create(&idhilos_consumidores[i], NULL, (void *)consumidor, &num_consumido) != 0)
        {
            perror("Error en el create cn\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_PROD; i++)
    {
        if (pthread_join(idhilos_productores[i], NULL) != 0)
        {
            perror("Error en el join pr\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_CONS; i++)
    {
        if (pthread_join(idhilos_consumidores[i], NULL) != 0)
        {
            perror("Error en el join cn\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    if (sem_destroy(&s) != 0)
    {
        perror("Error al destruir s\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&pr) != 0)
    {
        perror("Error al destruir pr\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&cn) != 0)
    {
        perror("Error al destruir cn\n");
        printf("Valor del errno %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Estado final del buffer para propósitos de depuración
    printf("Estado final del buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}