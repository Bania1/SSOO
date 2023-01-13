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
Implementar un programa que reciba por lineas de argumento 2 numeros, el primer numero representa 
el numero de hilos sumadores a crear, mientras que el segundo numero representa el numero de hilos 
restadores,variable global=0, tiene que guardar el resultado dependiendo habiendo sumado y restado,
se generan numeros aleatorios del 1 al 10000.
se debe de comprobar que el valor este correcto 
*/

int global=0;
pthread_mutex_t semaforo;

void *suma_bunda(int *sumita)
{
    for (int i = 0; i < (*sumita); i++)
    {
        if (pthread_mutex_lock(&semaforo) != 0)
        {
            printf("Error, pthread lock suma\n");
            exit(EXIT_FAILURE);
        }
        
        global++;

        if (pthread_mutex_unlock(&semaforo) != 0)
        {
            printf("Error, pthread unlock suma\n");
            exit(EXIT_FAILURE);
        }
    }
}

void *resta(int *restesita)
{
    for (int i = 0; i < (*restesita); i++)
    {
        if (pthread_mutex_lock(&semaforo) != 0)
        {
            printf("Error, pthread lock resta\n");
            exit(EXIT_FAILURE);
        }
        
        global--;

        if (pthread_mutex_unlock(&semaforo) != 0)
        {
            printf("Error, pthread unlock resta\n");
            exit(EXIT_FAILURE);
        }
    }
    
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error, poned 3 argumentos\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error, pthread mutex init\n");
        exit(EXIT_FAILURE);
    }

//Variables
    int sumador = atoi(argv[1]);
    int restador = atoi(argv[2]);

//Vectores
    int elemento_suma[sumador];
    int elemento_resta[restador];

//Variable que comprueba
    int correcion=0;

    srand(time(NULL));

//ID de hilos
    pthread_t hilo_suma[sumador];
    pthread_t hilo_resta[restador];

//Creamos los hilos

    //Sumadores
    for (int i = 0; i < sumador; i++)
    {
        elemento_suma[i]=rand()%10000;
        correcion += elemento_suma[i];

        if (pthread_create(&hilo_suma[i], NULL, (void *)suma_bunda, &elemento_suma[i]) != 0)
        {
            printf("Error, pthread create suma\n");
            exit(EXIT_FAILURE);
        }
    }

    //Restadores
    for (int i = 0; i < restador; i++)
    {
        elemento_resta[i]=rand()%10000;
        correcion -= elemento_resta[i];

        if (pthread_create(&hilo_resta[i], NULL, (void *)suma_bunda, &elemento_resta[i]) != 0)
        {
            printf("Error, pthread create resta\n");
            exit(EXIT_FAILURE);
        }
    }

//Esperamos a los hilos

    //Espera suma
    for (int i = 0; i < sumador; i++)
    {
        if (pthread_join(hilo_suma[i], NULL) != 0)
        {
            printf("Error, pthread join suma\n");
            exit(EXIT_FAILURE);
        }
    }

    //Espera resta
    for (int i = 0; i < restador; i++)
    {
        if (pthread_join(hilo_resta[i], NULL) != 0)
        {
            printf("Error, pthread join resta\n");
            exit(EXIT_FAILURE);
        }
    }
    
//Destruyendi el semaforo
    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error, pthread destroy \n");
        exit(EXIT_FAILURE);
    }

//Control  
    if (correcion != global)
    {
        printf("CORRECTO\n");
        exit(EXIT_SUCCESS);
    }
    
    else
    {
        printf("INCORRECTO\n");
        exit(EXIT_FAILURE);
    }
}