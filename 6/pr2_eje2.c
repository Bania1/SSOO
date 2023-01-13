#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

void *contarLinea(char *nf)
{
    FILE *fich=fopen(nf, "r");
    if (fich == NULL)
    {
        printf("Error en abrir el fichero\n");
        exit(EXIT_FAILURE);
    }
    int cont=0;
    char linea[200];

    while (fgets(linea, 200, fich))
    {
        cont++;
    }
    
    fclose(fich);

    int *retorno = (int *)malloc(sizeof(int));
    (*retorno) = cont;

    printf("El fichero %s tiene %d lineas\n", nf, cont);
    pthread_exit((void*)retorno);
}

//---------------------------------------------------------

int main(int argc, char **argv)
{
    if (argc==1)
    {
        printf("Error, introduce los ficheros\n");
        exit(EXIT_FAILURE);
    }
    int n=argc-1;

//Variables de id
    pthread_t idhilos[n];

//Crea hilos
    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&idhilos[i], NULL, (void*)contarLinea, argv[i+1]) != 0)
        {
            printf("Error, ha petado en create\n");
            exit(EXIT_FAILURE);
        }
    }
    
    int *recogidos, suma = 0;

//Espera
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i], (void**)&recogidos)!=0)
        {
            printf("Error, ha petado en join\n");
            exit(EXIT_FAILURE);
        }

        suma += (*recogidos);
        free(recogidos);
    }

    printf("\n\nLa suma total de lineas de todos los ficheros es de: %d\n", suma);

    exit(EXIT_SUCCESS);
}
//devuelven 0 si funcionan correctamente