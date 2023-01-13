#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc==1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    
    int n=atoi(argv[1]), fact=1;

    if (n>=0)
    {
        for (int i = 1; i <= n; i++)
        {
            printf("Factorial de %d --> %d x %d = %d\n", n, fact, i, fact*i);
            fact=fact*i;
            sleep(1);
        }
        printf("El factorial de %d es %d\n", n, fact);
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Introduce un numero valido puto\n");
        exit(EXIT_FAILURE);
    }
    
}