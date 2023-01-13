#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc<3)
    {
        printf("Error, implementa los ficheros\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid, flag;
    int status;
    
    //printf("[PADRE] ---> %ld\n", (long int)getpid());
    
    for (int i = 0; i < 2; i++)
    {
        pid=fork(); //Creamos el hijo
        if (pid==-1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid==0) //Inicio el hijo
        {
            if (i==0)
            {
                //printf("[HIJO] ---> %ld [PADRE] ---> %ld\n", (long int)getpid(), (long int)getppid());
                execvp("./factorial", &argv[i]);
                exit(EXIT_SUCCESS); 
            }
            else
            {
                //printf("[HIJO] ---> %ld [PADRE] ---> %ld\n", (long int)getpid(), (long int)getppid());
                execvp("./factorial", &argv[i]);
                exit(EXIT_SUCCESS);
            }   
        }
    }
    
    //Espera del padre
    while((flag=wait(&status))>0)
    {
        
        if(WIFEXITED(status))
        {
            printf("hijo %ld finalizado con status %d\n",(long int)flag,WEXITSTATUS(status));
        }
        else if(WIFSIGNALED(status))
        {
            printf("hijo %ld finalizado tras recibir una senal con status %d\n",(long int)flag,WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
            printf("hijo %ld parado con status %d\n",(long int)flag,WSTOPSIG(status));
        }
        else if(WIFCONTINUED(status))
        {
            printf("hijo %ld reanudado\n",(long int)flag);
        }

    }
    if(flag==(pid_t)-1 && errno==ECHILD)
    { 
        printf("Valor del errno= %d, definido como %s\n",errno,strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal\n");
        exit(EXIT_FAILURE);
    } 

    exit(EXIT_SUCCESS);
    
    return 0;
}
