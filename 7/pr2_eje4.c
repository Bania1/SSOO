#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

/*
Implemente un programa que cree dos hebras y cada una incremente 10000 veces en un bucle
una variable global (recuerde que la variable global, al estar en el mismo espacio de memoria para
las dos hebras, es compartida, y que su uso es “peligroso”). Imprima al final del programa principal
el valor de la variable (en cada ejecución posiblemente obtenga un valor diferente a 20000 –
problema de concurrencia –). Intente razonar el resultado, el cual le servirá como concepto
introductorio de la siguiente práctica de la asignatura
*/

//SEMAFORO BINARIO

int global = 0; //seccion critica
pthread_mutex_t semaforo;

void *funcionHilo()
{
    for (int i = 0; i < 10000; i++)
    {
        if(pthread_mutex_lock (&semaforo) != 0)
        {
            printf("Error en lock\n");
            exit(EXIT_FAILURE);
        }
        global++;
        if(pthread_mutex_unlock(&semaforo) != 0)
        {
            printf("Error en unlock\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main()
{
    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en init\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_t idhilos[2];

    for (int i = 0; i < 2; i++)
    {
        if(pthread_create(&idhilos[i], NULL, (void *)funcionHilo, NULL) != 0)
        {
            printf("Error en create\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(pthread_join(idhilos[i], NULL) != 0)
        {
            printf("Error en join\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("La variable global vale %d\n", global);
    
    if(pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error en destroy\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}