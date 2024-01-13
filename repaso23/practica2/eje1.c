#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

void *suma()
{
    float n1 = rand() % 10, n2 = rand() % 10;
    float suma_n = n1 + n2;

    printf("[HILO %lu] ---> He generado el numero: %f\n", pthread_self(), n1);
    printf("[HILO %lu] ---> He generado el numero: %f\n", pthread_self(), n2);
    printf("\n[HILO %lu] ---> La suma de los dos numeros es: %f\n", pthread_self(), suma_n);

    // Reservo memoria para la variable con la suma
    float *retorno = (float *)malloc(sizeof(float));
    (*retorno) = suma_n;

    // Devuelvo retorno cerrando los hilos
    pthread_exit((void *)retorno);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error al ejecutar el fichero, introduzca numero de hilos a crear\n");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    // Declaro la semilla de rand
    srand(time(NULL));

    // Id de los hilos
    pthread_t idhilos[n];

    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void *)suma, NULL) != 0)
        {
            printf("Error al crear el hilo\n");
            exit(EXIT_SUCCESS);
        }
    }

    float *recogido, suma = 0;

    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i], (void **)&recogido) != 0)
        {
            printf("Error en la espera del hilo\n");
            exit(EXIT_FAILURE);
        }

        // Almacenando las sumas y limpiando la variable "recogido"
        suma += (*recogido);
        free(recogido);
    }

    printf("\nLa suma total recogida por las hebras: %f\n", suma);

    exit(EXIT_SUCCESS);
}