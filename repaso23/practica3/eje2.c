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

Implemente un programa que cree un número N de hilos, donde N será un argumento al programa.
Tenga en cuenta los siguientes supuestos y condiciones:

 Los N hilos comparten dos variables, una llamada "par" e "impar" inicializadas a 0.

 Los hilos deben recibir un valor entero de 1 a N que indica el orden en el cuál se ha creado
el hilo. Así, el primer hilo creado recibe 1, el segundo 2, etc.

 Cada hilo debe generar 5 números aleatorios entre 0 y 10.

 Los hilos de índice de creación par deben ir sumando a la variable "par" los números que
generan.

 Los hilos de índice de creación impar deben ir sumando a la variable "impar" los números
que generan.

 Una vez finalizado cada hilo, éste debe devolver al programa principal la suma total de los
números que ha generado.

 El programa principal debe mostrar la suma devuelta por cada hebra y el valor final de las
variables “par” e “impar”.

*/

// Creo el semaforo para evitar condiciones de carrera
pthread_mutex_t sem;

// Variables que compartidas
int par = 0;
int impar = 0;

void *par_impar()
{
    int *suma = (int *)malloc(sizeof(int));
    (*suma) = 0;

    // Creo vector de numeros random
    int buffer[5];

    for (int i = 0; i < 5; i++)
    {
        buffer[i] = rand() % 10; // Voy rellenando el vector

        (*suma) += buffer[i]; // Voy sumando los valores del buffer
    }

    // Entra en seccion critica
    if (pthread_mutex_lock(&sem) != 0)
    {
        printf("Error al bloquear semaforo\n");
        exit(EXIT_FAILURE);
    }

    //-------------------------------------

    // Si es impar
    if (((*suma) % 2) != 0)
    {
        for (int i = 0; i < (*suma); i++)
        {
            impar++;
        }
    }
    // Si es par
    else
    {
        for (int i = 0; i < (*suma); i++)
        {
            par++;
        }
    }

    //-------------------------------------

    // Sale de la seccion critica
    if (pthread_mutex_unlock(&sem) != 0)
    {
        printf("Error al desbloquear semaforo\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void **)suma);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error al ejecutar el programa\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);

    // Semilla para numeros aleatorios
    srand(time(NULL));

    // Inicalizo semaforo
    if (pthread_mutex_init(&sem, NULL) != 0)
    {
        printf("Error al incializar el semfaro\n");
        exit(EXIT_FAILURE);
    }

    // Id de hilos
    pthread_t idhilos[N];

    int *valor_recogido;

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void *)par_impar, NULL) != 0)
        {
            printf("Error al crear la hebra\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(idhilos[i], (void **)&valor_recogido) != 0)
        {
            printf("Error al esperar las hebras\n");
            exit(EXIT_FAILURE);
        }

        printf("(Main)... La hebra de creacion %d devolvio el valor de suma: %d\n", i + 1, (*valor_recogido));

        free(valor_recogido);
    }

    printf("\n(Main)... Valor de la variable compartida impar: %d\n", impar);
    printf("\n(Main)... Valor de la variable compartida par: %d\n", par);

    if (pthread_mutex_destroy(&sem) != 0)
    {
        printf("Error al destruir el semaforo\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}