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
Implemente un programa que cree un vector de 10 elementos relleno con números aleatorios
entre 1 y 9. No cree el vector de manera global, declárelo en el main(). Reparta ese vector entre 2
hebras o 5 hebras a partes iguales, según se indique por linea de argumentos un 2 o un 5, de forma
que cada hebra sume la parte del vector que le corresponda y se lo devuelva al hilo principal, el cual
mostrará la suma de los resultados devuelto por las hebras creadas.
Ejemplo de invocación del programa para crear 5 hebras que se repartan el vector: ./a.out 5
*/

struct param
{
    int num_hebras;
    int num_aleatorio;
    int suma;
};


void *funcion(struct param *vector)
{
    int *retorna=(int *)malloc(sizeof(int));
    (*retorna) += vector->num_aleatorio; 
    vector->suma=0;

    if (vector->num_hebras == 2)
    {
        for (int i = 0; i < 10; i++)
        {
            vector->suma += vector->num_aleatorio;
        }
        
    }
    
    
    printf("Soy la hebra y me corresponde esta parte del vector ...");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error, introduce 2 o 5 para crear las hebras\n");
        exit(EXIT_FAILURE);
    }
    
    int hebras = atoi(argv[1]);

    srand(time(NULL));

//declaramos struct
    struct param vector[10];
    vector->num_hebras = hebras; //aqui ponemos el numero de hebras

//Rellenamos los valores del vector
    for (int i = 0; i < 10; i++)
    {
        vector[i].num_aleatorio=rand()%10;
    }
    

    pthread_t hilos[hebras];

    for (int i = 0; i < hebras; i++)
    {
        if (pthread_create(&hilos[i], NULL, (void *)funcion, &vector[i]) != 0 && hebras == 2)
        {
            printf("Error en pthread create 2\n");
            exit(EXIT_FAILURE);
        }

        else if (pthread_create(&hilos[i], NULL, (void *)funcion, &vector[i]) != 0 && hebras == 5)
        {
            printf("Error en pthread create 5\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < hebras; i++)
    {
        if (pthread_join(hilos[i], NULL) != 0 && hebras == 2)
        {
            printf("Error en pthread join 2\n");
            exit(EXIT_FAILURE);
        }
        else if (pthread_join(hilos[i], NULL) != 0 && hebras == 5)
        {
            printf("Error en pthread join 5\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("La suma total de los elementos del vector es de ...\n");
    exit(EXIT_SUCCESS);
    
}