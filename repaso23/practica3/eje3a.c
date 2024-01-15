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

Implemente el problema para hilos, teniendo en cuenta que la sección crítica va a ser un
array de enteros con una capacidad de 5 elementos y donde habrá un productor y un
consumidor. Cada uno podrá producir-consumir 10 elementos

*/

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE] = {0};

int indice_prod = 0;
int indice_cons = 0;

sem_t s;  // Semáforo general a 1 (o binario) para la sección crítica
sem_t cn; //  Semáforo general para el consumidor. Cuenta los espacios ocupados
sem_t pr; // Semáforo general para el productor. Cuenta los espacios libres.

void *productor()
{
    int producido;
    for (producido = 0; producido < NUM_ITEMS; ++producido)
    {
        int item = producido;

        // Esperamos a que no hayan clientes
        if (sem_wait(&pr) != 0)
        {
            perror("Error en wait cn\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // Entramos a la seccion critica
        if (sem_wait(&s) != 0)
        {
            perror("Error en wait s\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        buffer[indice_prod] = item;
        indice_prod = (indice_prod + 1) % BUFFER_SIZE;

        printf("------> El [Proveedor %d] ha producido el item %d en la posicion %d\n", producido, item, indice_prod);

        // Salimos de la seccion critica
        if (sem_post(&s) != 0)
        {
            perror("Error en post s\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // Incrementamos el numero de espacios dispobibles para consumir
        if (sem_post(&cn) != 0)
        {
            perror("Error en post cn\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

void *consumidor()
{
    int consumido;

    for (consumido = 0; consumido < NUM_ITEMS; ++consumido)
    {
        // Esperamos a que el proveedor termine de rellenar el buffer y haya espacio libre para consumir
        if (sem_wait(&cn) != 0)
        {
            perror("Error en wait cn\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // Entramos en la seccion critica
        if (sem_wait(&s) != 0)
        {
            perror("Error en wait s\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // Tomamos el item del buffer
        // indice_cons = 0;
        // indice_cons = rand() % 10 + 1;

        int item = buffer[indice_cons];
        indice_cons = (indice_cons + 1) % BUFFER_SIZE;

        printf("---> El [Consumidor %d] ha consumido el item %d en la posicion %d\n\n", consumido, item, indice_cons);

        // Salimos de la seccion critica
        if (sem_post(&s) != 0)
        {
            perror("Error en post s\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // Incrementamos el numero de espacios que se pueden ocupar para rellenar el buffer
        if (sem_post(&pr) != 0)
        {
            perror("Error en post pr\n");
            printf("Valor del errno %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit((void *)NULL);
}

int main()
{
    // semaforo
    if (sem_init(&s, 0, 1) != 0)
    {
        printf("Error en inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }

    // semaforo proveedor
    if (sem_init(&pr, 0, BUFFER_SIZE) != 0)
    {
        printf("Error en inicializar semaforo pr\n");
        exit(EXIT_FAILURE);
    }

    // semaforo consumidor
    if (sem_init(&cn, 0, 0) != 0)
    {
        printf("Error en inicializar semaforo cn\n");
        exit(EXIT_FAILURE);
    }

    // ids de hilo
    pthread_t idhilos_cn;
    pthread_t idhilos_pr;

    // Create
    if (pthread_create(&idhilos_pr, NULL, (void *)productor, NULL) != 0)
    {
        printf("Error, create proveedor\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&idhilos_cn, NULL, (void *)consumidor, NULL) != 0)
    {
        printf("Error, create consumidor\n");
        exit(EXIT_FAILURE);
    }

    // Join
    if (pthread_join(idhilos_pr, NULL) != 0)
    {
        printf("Error, join proveedor\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(idhilos_cn, NULL) != 0)
    {
        printf("Error, join consumidor\n");
        exit(EXIT_FAILURE);
    }

    // Destruimos los semaforos
    if (sem_destroy(&s) != 0)
    {
        printf("Error en el destroy s\n");
        exit(EXIT_FAILURE);
    }
    else if (sem_destroy(&cn) != 0)
    {
        printf("Error en el destroy cn\n");
        exit(EXIT_FAILURE);
    }
    else if (sem_destroy(&pr) != 0)
    {
        printf("Error en el destroy pr\n");
        exit(EXIT_FAILURE);
    }

    // Resultado
    printf("\nEstado final del buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}