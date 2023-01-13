#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int global = 0;

pthread_mutex_t sem;

void * sumadores(int * n)
{
    for (size_t i = 0; i < (*n); i++)
    {
        if(pthread_mutex_lock(&sem)!=0)
        {
            printf("Error en pthread_mutex_lock\n");
            exit(EXIT_FAILURE);
        }

        global++;

        if(pthread_mutex_unlock(&sem)!=0)
        {
            printf("Error en pthread_mutex_unlock\n");
            exit(EXIT_FAILURE);
        }
    }
    
}

void * restadores(int * n)
{
    for (size_t i = 0; i < (*n); i++)
    {
        if(pthread_mutex_lock(&sem)!=0)
        {
            printf("Error en pthread_mutex_lock\n");
            exit(EXIT_FAILURE);
        }

        global--;

        if(pthread_mutex_unlock(&sem)!=0)
        {
            printf("Error en pthread_mutex_unlock\n");
            exit(EXIT_FAILURE);
        }
    }
    
}


int main(int argc, char ** argv)
{
    if(argc!=3)
    {
        printf("Error en línea de argumentos, debe introducir dos números obligatoriamente.\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_mutex_init(&sem,NULL)!=0)
    {
        printf("Error al inicializar el semáforo.\n");
        exit(EXIT_FAILURE);
    }

    int numero_sumadores = atoi(argv[1]);
    int numero_restadores = atoi(argv[2]);
    pthread_t ids_sumadores[numero_sumadores];
    pthread_t ids_restadores[numero_restadores];
    int valores_sumadores[numero_sumadores];
    int valores_restadores[numero_restadores];
    int resultado_correcto = 0;

    srand(time(NULL));

    for (size_t i = 0; i < numero_sumadores; i++)
    {
        valores_sumadores[i] = rand()%1000;
        resultado_correcto += valores_sumadores[i];
        if(pthread_create(&ids_sumadores[i],NULL,(void*)sumadores,&valores_sumadores[i])!=0)
        {
            printf("Error en el create de los sumadores.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < numero_restadores; i++)
    {
        valores_restadores[i] = rand()%1000;
        resultado_correcto -= valores_restadores[i];
        if(pthread_create(&ids_restadores[i],NULL,(void*)restadores,&valores_restadores[i])!=0)
        {
            printf("Error en el create de los restadores.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < numero_sumadores; i++)
    {
        if(pthread_join(ids_sumadores[i],NULL)!=0)
        {
            printf("Error en el join de los sumadores.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < numero_restadores; i++)
    {
        if(pthread_join(ids_restadores[i],NULL)!=0)
        {
            printf("Error en el join de los restadores.\n");
            exit(EXIT_FAILURE);
        }
    }

    if(pthread_mutex_destroy(&sem)!=0)
    {
        printf("Error al destruir el semáforo.\n");
        exit(EXIT_FAILURE);
    }

    if(global==resultado_correcto)
    {
        printf("Resultado correcto.\n");
    }
    else
    {
        printf("Resultado incorrecto.\n");
    }
    
    exit(EXIT_SUCCESS);
    
}
