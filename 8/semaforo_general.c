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
#include <semaphore.h>

Semaforo genrales/ o contador;
    sem_t semaforo

inicializar semaforo:
    sem_init(&semaforo, 0, x);  la x es el valor del semaforo

Decrementa semaforo (lock):
    sem_wait(&semaforo);

Incrementa semaforo (unlock):
    sem_post(&semaforo);

Eliminar semaforo:
    sem_destroy(&semaforo);
*/

/*
Enunciado:

    Implementar un programa que por linea de argumentos obtenga el numero de productores
    y el numero de consumidores. Se tiene que ultilizar semaforos generales o contadores
    para sincronizar los procesos. El programa debe crear los procesos productores y
    consumidores y sincronizarlos para que no se produzcan errores de concurrencia.
*/

int buffer[5] = {0, 0, 0, 0, 0};

sem_t s, n, e;

void *productor()
{
    if (sem_wait(&e) != 0)
    {
        printf("Error en sem wait e\n");
        exit(EXIT_FAILURE);
    }
//-------------
    if (sem_wait(&s) != 0)
    {
        printf("Error en sem wait s\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++)
    {
        buffer[i]++;
    }
  
    if (sem_post(&s) != 0)
    {
        printf("Error en sem post s\n");
        exit(EXIT_FAILURE);
    }
//--------------
    if (sem_post(&n) != 0)
    {
        printf("Error en sem post n\n");
        exit(EXIT_FAILURE);
    }
}

void *consumidor()
{
    if (sem_wait(&n) != 0)
    {
        printf("Error en sem wait n\n");
        exit(EXIT_FAILURE);
    }
//-------------
    if (sem_wait(&s) != 0)
    {
        printf("Error en sem wait s\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++)
    {
        buffer[i]--;
    }
  
    if (sem_post(&s) != 0)
    {
        printf("Error en sem post s\n");
        exit(EXIT_FAILURE);
    }
//--------------
    if (sem_post(&e) != 0)
    {
        printf("Error en sem post e\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error introduce los argumentos bien\n");
        exit(EXIT_FAILURE);
    }
    
    if (sem_init(&s, 0, 1) != 0)
    {
        printf("Error en inicializar semaforo s\n");
        exit(EXIT_FAILURE);
    }
    
    if (sem_init(&n, 0, 0) != 0)
    {
        printf("Error en inicializar semaforo n\n");
        exit(EXIT_FAILURE);
    }
    
    if (sem_init(&e, 0, 5) != 0)
    {
        printf("Error en inicializar semaforo e\n");
        exit(EXIT_FAILURE);
    }

//Tamaños
    int num_productores=atoi(argv[1]);
    int num_consumidores=atoi(argv[2]);

//ids de hilos
    pthread_t id_productores[num_productores];
    pthread_t id_consumidores[num_consumidores];

//no se 
    /*int valores_productor[num_productores];
    int valores_consumidor[num_consumidores];*/

//Creates
    for (int i = 0; i < num_productores; i++)
    {
        if (pthread_create(&id_productores[i], NULL, (void *)&productor, NULL) != 0)
        {
            printf("Error en pthread create productor\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_consumidores; i++)
    {
        if (pthread_create(&id_consumidores[i], NULL, (void *)&consumidor, NULL) != 0)
        {
            printf("Error en pthread create consumidor\n");
            exit(EXIT_FAILURE);
        }
    }

//Joins
    for (int i = 0; i < num_productores; i++)
    {
        if (pthread_join(id_productores[i], NULL) != 0)
        {
            printf("Error en pthread join productor\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_consumidores; i++)
    {
        if (pthread_join(id_consumidores[i], NULL) != 0)
        {
            printf("Error en pthread join consumidor\n");
            exit(EXIT_FAILURE);
        }
    }
    
//destroys
    if (sem_destroy(&s) != 0)
    {
        printf("Error en destroy s\n");
        exit(EXIT_FAILURE);
    }
    else if (sem_destroy(&n) != 0)
    {
        printf("Error en destroy s\n");
        exit(EXIT_FAILURE);
    }
    else if (sem_destroy(&e) != 0)
    {
        printf("Error en destroy s\n");
        exit(EXIT_FAILURE);
    }

//resultado
    for (int i = 0; i < 5; i++)
    {
        printf("El valor del buffer es de: %d\n", buffer[i]);
    }

    exit(EXIT_SUCCESS);
}