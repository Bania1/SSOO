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
    Creacion de hilos:

        1º Variable/s donde guardar el id del hilo o hilos
        pthread_t, idhilo1, idhilo2,

        2º Crear los hilos: Recibe 4 argumentos
        funcion: pthread_create(1º(&idhilo1),2º(NULL),3º((void*)imprime),4º(cadena1));

            1º Variable por referencia donde se almacena el id del hilo
            
            2º NULL --> siempre --> parametros por defecto
            
            3º (void*) nombre_de_la_funcion asociada al hilo, devuelve un puntero
               de tipo vacio y //return NULL;

            4º Direccion de memoria del argumento de la funcion asociada al hilo

        3º Creamos el segundo hilo
        funcion: pthread-create(&idhilo2, NULL, (void*)imprime, cadena2);

        4º Espera
        funcion: pthread_join(1º(idhilo1),2º(void**)NULL);
                 pthread_join(idhilo2,(void**)NULL);

            1º Variable que almacena el id del hilo a esperar

            2º (void**)NULL--> direccion de memoria de un puntero que recoge el valor
               retornado por el hilo     

            3º exit(EXIT_SUCCESS);

----------------IMPORTANTE COMPILAR CON LA LIBRERIA -LPTHREAD------------------------

*/

void imprime(char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        printf("%c\n", cadena[i]);
        sleep(1);
    }
    pthread_exit((void*)NULL);
}

int main()
{
    char cadena1[]="Hola";
    char cadena2[]="Mundo";

//Variables de id de hilos    
    pthread_t idhilo1, idhilo2;

//Crea hilos
    pthread_create(&idhilo1, NULL, (void*)imprime, cadena1);
    pthread_create(&idhilo1, NULL, (void*)imprime, cadena2);

//Espera
    pthread_join(idhilo1,(void**)NULL);
    pthread_join(idhilo2,(void**)NULL);
    
    exit(EXIT_SUCCESS);
}
