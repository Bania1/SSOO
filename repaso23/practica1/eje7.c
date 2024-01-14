#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

/*

Realizar un programa que capture la señal de alarma, de manera que imprima la cadena “RING”
pasados 5 segundos, después pasados otros 3 segundos y por último cada segundo. Implementar
esto último, utilizando un bucle infinito que vaya imprimiendo el número de timbrazos. Pasados 4
timbrazos, el proceso se debe parar utilizando para ello la función kill().

*/

void primerManejador(int sig);
void segundoManejador(int sig);
void ultimoManejador(int sig);

int main()
{
    signal(SIGALRM, primerManejador);

    alarm(5);

    while (1)
    {
        pause();
    }

    return 0;
    // exit(EXIT_SUCCESS);
}

void ultimoManejador(int sig)
{
    kill(getpid(), SIGTERM);
}

void segundoManejador(int sig)
{
    static int cont = 0;
    cont++;

    printf("RING\n");

    if (cont >= 3)
    {
        signal(SIGALRM, ultimoManejador);
    }

    alarm(1);
}

void primerManejador(int sig)
{
    printf("RING\n");

    signal(SIGALRM, segundoManejador);

    alarm(3);
}