#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

/*

Implemente un programa que cuente las líneas de los ficheros de texto que se le pasen como
parámetros y al final muestre el número de líneas totales (contando las de todos los ficheros juntos).

Ejemplo de llamada: ./a.out fichero1 fichero2 fichero3
Debe crear un hilo por cada fichero indicado por linea de argumentos, de forma que todos los
ficheros se procesen de manera paralela, uno por cada hilo.

*/

void *contLineasFich(char *nf)
{
    FILE *fich = fopen(nf, "r");
    if (fich == NULL)
    {
        printf("Error, no hay lineas en fichero\n");
        exit(EXIT_FAILURE);
    }

    int cont = 0;
    char linea[200];

    while (fgets(linea, 200, fich))
    {
        cont++;
    }

    fclose(fich);

    printf("El fichero %s tiene %d lineas\n", nf, cont);

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = cont;

    pthread_exit((void *)retorno);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Error, include los ficheros que quieras contar las lineas\n");
        exit(EXIT_FAILURE);
    }

    int n = argc - 1; // Numero de ficheros que pasamos por linea de argumentos

    pthread_t idhilos[n];

    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void *)contLineasFich, argv[i + 1]) != 0)
        {
            printf("Error al crear la hebra del fichero\n");
            exit(EXIT_FAILURE);
        }
    }

    int *recogida, suma = 0;

    // Espera
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i], (void **)&recogida) != 0)
        {
            printf("Error, ha petado en join\n");
            exit(EXIT_FAILURE);
        }

        suma += (*recogida);
        free(recogida);
    }

    printf("\nLa suma total de las lineas de todos los ficheros es de: %d\n", suma);
    exit(EXIT_SUCCESS);
}