#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

/*

Realizar un programa que este permanentemente a la espera de capturar una señal SIGUSR1 (en un
bucle while(1) infinito por ejemplo), de forma que cuando la capture imprima su ID. Compílelo y
láncelo por consola.

Creo otro programa aparte que reciba por línea de argumentos un ID de un proceso, y su cometido
sea enviar una señal SIGUSR1 al proceso cuyo ID ha recibido por dicha línea de argumentos. Una
vez enviada la señal SIGUSR1 esperará durante 1 segundo y enviará al mismo proceso al que envió
la señal SIGUSR1 la señal de KILL.

Compruebe por consola si el proceso al que ha enviado ambas
señales existe ya en el sistema

*/

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error, pon el pid\n");
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);

    kill(pid, SIGUSR1);

    sleep(1);

    kill(pid, SIGKILL);

    sleep(1);

    exit(EXIT_SUCCESS);
}