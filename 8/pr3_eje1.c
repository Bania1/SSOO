#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int *buffer;
pthread_mutex_t semaforo;

//Productores
void *funcion_suministros(int *M)
{
    int modelo = rand()%(*M);
    int cantidad = rand()%10 + 1;

    if (pthread_mutex_lock(&semaforo) != 0)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    
    buffer[modelo] += cantidad;
    printf("--> Soy el proceso suministrador %lu, suministro %d unidades de la camiseta %d.\n", pthread_self(), cantidad, modelo);
    
    if (pthread_mutex_unlock(&semaforo) != 0)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void*)NULL);
}

//Consumidores
void *funcion_consumidores(int *M)
{
    int modelo = rand()%(*M);
    int cantidad = rand()%10 + 1;

    if (pthread_mutex_lock(&semaforo) != 0)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    
    buffer[modelo] -= cantidad;
    printf("<-- Soy el proceso consumidor %lu, compro %d unidades de la camiseta %d\n", pthread_self(), cantidad, modelo);

    if (pthread_mutex_unlock(&semaforo) != 0)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void*)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&semaforo, NULL) != 0)
    {
        printf("Error en sem\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int M = atoi(argv[1]); //Provedores
    int N = atoi(argv[2]);//Consumidores

    if (M < N)
    {
        printf("Error, M tiene que ser mayor que N\n");
        exit(EXIT_FAILURE);
    }
    
    //Reservo memoria del buffer y el tamaño es el numero de modelos
    buffer = (int *)malloc(sizeof(int)*M);

    //Rellenamos el vector para el stock de los modelos de camisetas
    for (int i = 0; i < M; i++)
    {
        buffer[i] = rand()%10 + 1;
    }
    
    //Mostramos el buffer
    for (int i = 0; i < M; i++)
    {
        printf("buffer[%d] --> %d \t", i, buffer[i]);
    }
    printf("\n\n");
    
    pthread_t suministro[M], consumidor[N];

    //Suministro create
    for (int i = 0; i < M; i++)
    {
        if (pthread_create(&suministro[i], NULL, (void *)funcion_suministros, &M) != 0)
        {
            printf("Error create provedor\n");
            exit(EXIT_FAILURE);
        } 
    }
    
    //Consumidor create
    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&consumidor[i], NULL, (void *)funcion_consumidores, &M) != 0)
        {
            printf("Error create cliente\n");
            exit(EXIT_FAILURE);
        } 
    }

    //Recogemos los resultados retornados
    //Suministro join
    for (int i = 0; i < M; i++)
    {
        if (pthread_join(suministro[i], NULL) != 0)
        {
            printf("Error join provedor\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //Consumidor
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(consumidor[i], NULL) != 0)
        {
            printf("Error join cliente\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\n\n");

     //Mostramos el buffer
    for (int i = 0; i < M; i++)
    {
        printf("buffer[%d] --> %d \t", i, buffer[i]);
    }

    printf("\n\n");

    if (pthread_mutex_destroy(&semaforo) != 0)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    
    free(buffer);
    exit(EXIT_SUCCESS);
}