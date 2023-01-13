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

IMPLEMENTAR UN PROGRAMA QUE RECIBA POR LINEA DE ARGUMENTOS OBLIGATORIAMENTE
EL NUMERO DE HILOS A CREAR, CADA HILO RECIBIRA 2 NUMEROS ALEATORIOS GENERADOS
POR EL MAIN Y DEBERA RETORNAR LA SUMA DE ESTOS ADEMAS DEBE DE IMPRIMIR SU ID
Y LA SUMA

HILO --> ID HILO, 1º NUMERO, 2º NUMERO, SUMA

*/

struct param
{
    int n1;
    int n2;
};

void *info(struct param *num)
{
    int suma_rand = num->n1 + num->n2;

    printf("Soy el hilo %lu y he generado estos numeros: %d, %d ---> su suma es de %d\n", pthread_self(), num->n1, num->n2, suma_rand);

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = suma_rand;
    pthread_exit((void *)retorno);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error, introduce un numero\n");
        exit(EXIT_FAILURE);
    }

//Funcion de rand y el argumento
    int n = atoi(argv[1]);
    srand(time(NULL));
    
// ID
    pthread_t idhilos[n];

//Struct con vector para cada hilo que se cree
    struct param p[n];

// Creamos los hilos
    for (int i = 0; i < n; i++)
    {
        p[i].n1 = rand()%10;
        p[i].n2 = rand()%10;

        if (pthread_create(&idhilos[i], NULL, (void*)info, &p[i]) != 0)
        {
            printf("Error en el pthread create\n");
            exit(EXIT_FAILURE);
        }
    }

//Puntero que recoge direcciones de memoria y variable que va almacenando la suma
    int *recogidos, suma = 0;

//Esperamos los hilos
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i],(void**)&recogidos) != 0)
        {
            printf("Error en el pthread join\n");
            exit(EXIT_FAILURE);
        }

    //Recogemos las sumas de las direcciones de memorias
        suma += (*recogidos);
        free(recogidos);
    }
    
    printf("\n\nLa suma total de todos los numeros es de: %d\n", suma);

    exit(EXIT_SUCCESS);

}