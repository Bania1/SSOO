#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int res = 1;

    if(n >= 0)
    {
        for (int i = 1; i <= n; i++)
        {
            printf("Factorial de [%d] --> %d x %d = %d\n", n, res, i, res*i);
            res *= i ;
            sleep(1);
        }
        printf("FACTORIAL DE [%d] ---> %d\n", n, res);
        exit(EXIT_SUCCESS);
    }
}