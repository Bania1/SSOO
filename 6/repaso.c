#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

struct param
{
    int *vector;        //vector
    int hebras;         //hebras
    int it;             //iterador
};

//-----------------------------------------------

void *bunda(struct param *v)
{
    int *suma_retorna = malloc(sizeof(int));
    (*suma_retorna)=0;

    struct param *v_aux;
    v_aux = v;                  //hice una copia de la estructura

//Para repartir el vector en 2 hebras:
    if (v_aux->hebras == 2)
    {
        for (int i = v_aux->it ; i < (v_aux->it)+5 ; i++)       //el vector llega del v[0] hasta v[4] y despues v[5] a v[10]
        {
            printf("Soy la hebra %lu y tengo la suma de esta parte del vector %d\n", pthread_self(), v_aux->vector[i]);
            (*suma_retorna) += v_aux->vector[i];
            sleep(1);
        }
        
        v_aux->it += 5;

        pthread_exit((void **)suma_retorna);
    }

//Para repartir el vector en 5 hebras
    else
    {
        for (int i = v_aux->it; i < (v_aux->it) + 2; i++)//el vector llega del v[0] hasta v[1] y despues v[2] a v[3], etc...
        {
            printf("Soy la hebra %lu y tengo la suma de esta parte del vector %d\n", pthread_self(), v_aux->vector[i]);
            (*suma_retorna) += v_aux->vector[i];
            sleep(1);
        }
        
        v_aux->it += 2;

        pthread_exit((void **)suma_retorna);
    }
}

void imprimeVector(int *vector)
{
    printf("Imprimiendo valores del vector...\n");
    for (int i = 0; i < 10; i++)
    {
        printf("V[i] ---> %d", vector[i]);
        sleep(1);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error, pase por argumentos un 2 o un 5\n");
        exit(EXIT_FAILURE);
    }

//Reserva memoria del struct
    struct param *v=malloc(sizeof(struct param));

//Declaramos los valores introducidos por linea de argumentos
    v->hebras = atoi(argv[1]);

//declaro el iterador del struct
    v->it = 0;

    if (v->hebras != 5 && v->hebras != 2)
    {
        printf("Error, tiene que ser un 2 o 5\n");
        exit(EXIT_FAILURE);
    }

//Reservo memoria del vector de enteros de 10 elementos    
    srand(time(NULL));
    v->vector = (int *)malloc(10*sizeof(int));

//IDs de los hilos
    pthread_t hilos[v->hebras];

//Variables de recogida y la suma
    int *suma_recogida;
    int suma=0;

//Creacion de los hilos
    for (int i = 0; i < v->hebras; i++)
    {
        v->vector[i] = ((rand()%9)+1);

        if (pthread_create(&hilos[i], NULL, (void *)bunda, &v[i]) != 0)
        {
            printf("Error al hacer el pthread create\n");
            exit(EXIT_FAILURE);
        }
    }
    
//Esperamo a los hilos
    for (int i = 0; i < v->hebras; i++)
    {
        if (pthread_join(hilos[i], ((void **)&suma_recogida)) != 0)
        {
            printf("Error en el pthread join\n");
            exit(EXIT_FAILURE);
        }
        
        suma += (*suma_recogida);
        free(suma_recogida);
    }
    
    printf("La suma de los numeros de todos los valores es: %d\n", suma);
    exit(EXIT_SUCCESS);
}