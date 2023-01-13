#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
    pid_t pid;
    pid=fork(); //proceso de clonacion 
                //a.out , hijo
                // si fork() devuelve -1 --> error
                //si devuelve 0 --> hijo
                //si devuelve >0 --> padre (pid del hijo)

    if (pid==-1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)
    {
        printf("Soy el hijo\n");
    }
    else
    {
        printf("Soy el padre\n");
    }    
    
    printf("Hola mundo\n");
    return 0;
}