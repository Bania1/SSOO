#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void bunda(int signal) //codigo de la señal
{ 
    printf("\nHe recibido la señal ctrl+c con codigo %d \n", signal);
    exit(EXIT_SUCCESS);
} 

int main()
{
    signal(SIGINT, &bunda);
    printf("Mi pid es %ld\n", (long int)getpid());
    while (1);
    
    exit(EXIT_SUCCESS);
}

/* 
#define SIGUSR1 10

SIGINT ---> CONTROL C PARA CERRAR EL PROGRAMA

1) TRATAMIENTO DE LA SEÑAL --> ASIGNAR UNA FUNCIONALIDAD ----> UNA FUNCION
FUNCION signal(1,2) --> PRIMER ARGUMENTO SEÑAL Y EL SEGUNDO LA FUNCION MANEJADORA

*/