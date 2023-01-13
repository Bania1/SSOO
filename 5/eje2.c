//programa que reciba por linea de argumentos un numero obligatoriamente
//el cual sera el numero de hilos a crear
//el proceso principal (a.out) debe crear un vector de 20 elementos
//y rellenarlo de forma aleatoria
//[0-9]
//srand(time (NULL)) --> siempre en el main, v[i]=rand()%10
//cada hilo debe calcular la suma de la porcion de vector que le haya tocado

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

struct param
{
    int inicio; 
    int fin;
    int vector[20];
};

void *suma(struct param *p)
{
    int suma=0;
    for (int i = p->inicio; i < p->fin; i++)
    {
        suma+=p->vector[i];
    }
    pthread_exit((void*)NULL);
}

int main(int argc, char **argv)
{
    if (argc!=1)
    {
        printf("Error, introduce el numero de hilos");
        exit(EXIT_FAILURE);
    }
    int n=argv[1];

    srand(time(NULL));
    struct param *p;

    for (int i = 0; i < n; i++)
    {
        p->inicio= i*(20/n);
        p->fin= p->inicio+(20/n);

        if (i==n-1)
        {
            p->fin=20;
        }
        
        p->vector[i]=rand()%20;
    }
//Id de hilos
    pthread_t idhilos[n];

//Creamos los hilos
    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void*)suma, argv[i+1]) != 0)
        {
            printf("Error, ha petado en create\n");
            exit(EXIT_FAILURE);
        }
    }
    
//Espera
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i], (void**)NULL) != 0)
        {
            printf("Error, ha petado en create\n");
            exit(EXIT_FAILURE);
        }
    } 
}