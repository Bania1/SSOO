#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define MAX_RANDOM 9
#define TAM_VECTOR 10

/*

Implemente un programa que cree un vector de 10 elementos relleno con números aleatorios
entre 1 y 9. No cree el vector de manera global, declárelo en el main().
Reparta ese vector entre 2 hebras o 5 hebras a partes iguales, según se indique por linea de argumentos un 2 o un 5, de forma
que cada hebra sume la parte del vector que le corresponda y se lo devuelva al hilo principal, el cual
mostrará la suma de los resultados devuelto por las hebras creadas.

Ejemplo de invocación del programa para crear 5 hebras que se repartan el vector: ./a.out 5

*/

struct array
{
    int inicio;  // Inicio
    int final;   // Final
    int *vector; // Vector
    int suma;    // Aqui cada hebra alamcenara su suma
};

void *manejador(struct array *vector)
{
    struct array *datos = vector;
    datos->suma = 0;

    for (int i = datos->inicio; i <= datos->final; i++)
    {
        datos->suma += datos->vector[i];
    }
}

void imprimeVector(struct array *vector, int comprobar)
{
    struct array *datos = vector;
    int sumita_comprobar = 0;
    printf("\nImprimiendo valores del vector...\n");

    for (int i = 0; i < 10; i++)
    {
        printf("v[%d] ---> %d\n", i + 1, datos->vector[i]);
        sumita_comprobar += datos->vector[i];
        sleep(1);
    }

    if (sumita_comprobar == comprobar)
    {
        printf("\nLa suma es correcta\n");
    }
    else
    {
        printf("\nLa suma es INcorrecta\n");
    }
}

int main(int argc, char **argv)
{
    if ((argc != 2) || ((atoi(argv[1]) != 2) && (atoi(argv[1]) != 5)))
    {
        printf("Error al abrir el ejecutable, introduzca un 2 o un 5\n");
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    int vector[TAM_VECTOR];
    srand(time(NULL));

    // Ponemos numeros random en el vector
    for (int i = 0; i < TAM_VECTOR; i++)
    {
        vector[i] = (rand() % MAX_RANDOM) + 1; // nº del 1 al 9
    }

    // Creamos las hebras
    pthread_t idhilos[n];

    struct array datos_hilos[n];

    int tam_seccion = TAM_VECTOR / n;

    for (int i = 0; i < n; i++)
    {
        datos_hilos[i].inicio = i * tam_seccion;
        datos_hilos[i].final = (i + 1) * tam_seccion - 1;
        datos_hilos[i].vector = vector;

        // Ajuste para la última hebra en caso de que el tamaño no sea divisible exactamente
        if (i == n - 1)
        {
            datos_hilos[i].final = TAM_VECTOR - 1;
        }

        if (pthread_create(&idhilos[i], NULL, (void *)manejador, &datos_hilos[i]) != 0)
        {
            printf("Error al crear las hebras\n");
            exit(EXIT_FAILURE);
        }
    }

    // Variables de recogida y la suma
    int suma_total = 0;

    // Esperamos a las hebras
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(idhilos[i], NULL) != 0)
        {
            printf("Error en el join\n");
            exit(EXIT_FAILURE);
        }

        suma_total += datos_hilos[i].suma;
    }

    printf("La suma de los numeros de todos los valores es: %d\n", suma_total);
    imprimeVector(datos_hilos, suma_total);

    exit(EXIT_SUCCESS);
}