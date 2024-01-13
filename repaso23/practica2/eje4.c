#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define VECES 10000
#define NUM_HEBRAS 2

/*

Implemente un programa que cree dos hebras y cada una incremente 10000 veces en un bucle
una variable global (recuerde que la variable global, al estar en el mismo espacio de memoria para
las dos hebras, es compartida, y que su uso es “peligroso”). Imprima al final del programa principal
el valor de la variable (en cada ejecución posiblemente obtenga un valor diferente a 20000 –
problema de concurrencia –). Intente razonar el resultado, el cual le servirá como concepto
introductorio de la siguiente práctica de la asignatura.

*/

int global = 0;

void *incrementar()
{
    for (int i = 0; i < VECES; i++)
    {
        global++;
    }
}

int main()
{
    pthread_t idhilos[NUM_HEBRAS];

    for (int i = 0; i < NUM_HEBRAS; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void *)incrementar, NULL) != 0)
        {
            printf("Error al crear las hebras\n");
            exit(EXIT_FAILURE);
        }
    }

    int *recogida, suma = 0;

    for (int i = 0; i < NUM_HEBRAS; i++)
    {
        if (pthread_join(idhilos[i], NULL))
        {
            printf("Error en la espera\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("La suma del incremento de la variable global es %d\n", global);
}