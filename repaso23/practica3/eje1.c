#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

/*

Una tienda que vende camisetas guarda en una base de datos (buffer) las cantidades de camisetas
que tiene según el modelo. Por ejemplo, un buffer de camisetas[5] indica que existen 5 modelos de
camisetas y cada elemento de este buffer guarda las cantidades iniciales de cada una de ellas.

Implementar un programa que genere N clientes (pedido por linea de argumentos) y M proveedores
(la misma cantidad de proveedores que modelos de camiseta, también pedido por linea de
argumentos).

Para simular una compra, cada hilo Cliente debe generar un valor aleatorio para el modelo de
camiseta que quiere comprar y otro para la cantidad a comprar. Esta cantidad debe decrementar el
stock de la camiseta en cuestión.

Para simular un suministro, cada Proveedor debe hacer lo mismo que el Cliente pero en este caso,
incrementando el stock de la camiseta en cuestión.

Utilice semáforos binarios para resolver este problema de concurrencia imprimiendo el buffer antes
de generar los hilos y al final del programa para comprobar que se ha ejecutado correctamente.
En cuanto al numero de camisetas a comprar o suministrar, por ejemplo pueden ser valores
aleatorios entre [1-10]. Para rellenar el vector inicial de stock de camisetas podría generar números
aleatorios entre [1-100]. Si se diera el caso de que algún cliente quiere comprar más camisetas de
las disponibles solo se le proporcionarán las que queden.

El ejercicio se puede implementar de diversas maneras. La salida que se muestra a continuación
como ejemplo ha hecho uso en la zona de variables globales de un puntero a un vector de enteros
(para el vector de camisetas) y de la creación e inicialización de un mutex.

*/

int *buffer;
pthread_mutex_t sem;

void *cliente(int *modelos)
{
    int prenda = (rand() % (*modelos));
    int cantidad = (rand() % 10) + 1;

    if (pthread_mutex_lock(&sem) != 0)
    {
        printf("Error en mutexlock cliente\n");
        exit(EXIT_FAILURE);
    }

    buffer[prenda] -= cantidad;
    printf("[CLIENTE %lu] ---> Comprare de la prenda %d, [%d] unidades\n", pthread_self(), prenda, cantidad);

    if (pthread_mutex_unlock(&sem) != 0)
    {
        printf("Error en mutexUnlock cliente\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit((void *)NULL);
}

void *proveedor(int *modelos)
{
    int prenda = (rand() % (*modelos));
    int cantidad = (rand() % 10) + 1;

    if (pthread_mutex_lock(&sem) != 0)
    {
        printf("Error en mutexlock proveedor\n");
        exit(EXIT_FAILURE);
    }

    buffer[prenda] += cantidad;
    printf("[PROVEEDOR %lu] ---> Suministrare de la prenda %d, [%d] unidades\n", pthread_self(), prenda, cantidad);

    if (pthread_mutex_unlock(&sem) != 0)
    {
        printf("Error en mutexUnlock proveedor\n");
        exit(EXIT_FAILURE);
    }
    pthread_exit((void *)NULL);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Error al ejecutatar el ejecutable\n");
        exit(EXIT_FAILURE);
    }

    // Iniciamos el semaforo
    if (pthread_mutex_init(&sem, NULL) != 0)
    {
        printf("Error al iniciar semaforo\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int N = atoi(argv[1]); // Nº clientes
    int M = atoi(argv[2]); // Nº proovedor

    if (N > M)
    {
        printf("El numero de clientes supera el numero de clientes tiene que ser menor que de proovedores\n");
        exit(EXIT_FAILURE);
    }

    // Reservo memoria del vector
    buffer = (int *)malloc(sizeof(int) * M);

    printf("\n");

    // Relleno el vector con modelos de camisetas con sus unidades
    for (int i = 0; i < M; i++)
    {
        buffer[i] = (rand() % 100) + 1;
    }

    // Mostramos el buffer de los modelos de camisetas
    for (int i = 0; i < M; i++)
    {
        printf("buffer[%d] = %d\t", i, buffer[i]);
    }
    printf("\n\n");

    // Ids de hebras
    pthread_t idhilos_cliente[N];
    pthread_t idhilos_proveedor[M];

    // Hebra proveedor create
    for (int i = 0; i < M; i++)
    {
        if (pthread_create(&idhilos_proveedor[i], NULL, (void *)proveedor, &M) != 0)
        {
            printf("Error al crear hebras proveedor\n");
            exit(EXIT_FAILURE);
        }
    }

    // Hebra cliente create
    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&idhilos_cliente[i], NULL, (void *)cliente, &M) != 0)
        {
            printf("Error al crear hebras cliente\n");
            exit(EXIT_FAILURE);
        }
    }

    // Hebra proveedor espera
    for (int i = 0; i < M; i++)
    {
        if (pthread_join(idhilos_proveedor[i], NULL) != 0)
        {
            printf("Error al esperar hebras proveedor\n");
            exit(EXIT_FAILURE);
        }
    }

    // Hebra cliente espera
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(idhilos_cliente[i], NULL) != 0)
        {
            printf("Error al esperar hebras cliente\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\n\n");

    // Mostramos de nuevo el vector de modelos de camisetas
    for (int i = 0; i < M; i++)
    {
        printf("buffer[%d] = %d\t", i, buffer[i]);
    }
    printf("\n\n");

    // Se destruye el semaforo
    if (pthread_mutex_destroy(&sem) != 0)
    {
        printf("Error al esperar hebras proveedor\n");
        exit(EXIT_FAILURE);
    }

    // Liberamos la memoria del vector de prendas
    free(buffer);
    exit(EXIT_SUCCESS);
}
