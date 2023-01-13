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
--------------------------------------------------------------------
RETORNA VALORES DE HILOS 
IMPLEMENTAR UN PROGRAMA QUE GENERE 2 HILOS, CADA HILO DEBE GENERAR UN NUMERO ALEATORIO.

ADEMAS DEBE IMPRIMIR POR PANTALLA SU ID Y EL NUMERO ALEATORIO GENERADO Y POR ULTIMO 
RETORNAR DICHO NUMERO ALEATORIO. 

EL PROCESO PRINCIPAL DEBE IMPRIMIR LA SUMA DE LOS NUMEROS ALEATORIOS DEVUELTOS POR LOS HILOS
--------------------------------------------------------------------
*/

void *funcion()
{
    int n=rand()%10;
    printf("Soy el hilo %lu y el numero generado es %d\n", pthread_self(), n);

    int *retorno=(int *)malloc(sizeof(int));    //Reserva memoria para un valor
    (*retorno)=n;

    pthread_exit((void *)retorno);
}

int main()
{
    srand(time(NULL));
    pthread_t idhilo1, idhilo2; //ID de hilos

//Creamos hilos
    if (pthread_create(&idhilo1, NULL, (void *)funcion, NULL) != 0)
    {
        printf("Error en pthread create\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&idhilo2, NULL, (void *)funcion, NULL) != 0)
    {
        printf("Error en pthread create\n");
        exit(EXIT_FAILURE);
    }
    
    int *recogido, suma=0;

    if (pthread_join(idhilo1, (void **)&recogido) != 0)
    {
        printf("Error en pthread join\n");
        exit(EXIT_FAILURE);
    }
    suma += (*recogido);

    free(recogido);

    if (pthread_join(idhilo2, (void **)&recogido) != 0)
    {
        printf("Error en pthread join\n");
        exit(EXIT_FAILURE);
    }
    
    suma += (*recogido);

    free(recogido);

    printf("Suma = %d\n", suma);
    
    exit(EXIT_SUCCESS);
}