#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main()
{
    pid_t pid, flag;
    int status;

    pid=fork();
    if (pid == -1)
    {
        printf("Error en el fork()\n");
		exit(EXIT_FAILURE);
    }
    else if (pid == 0) //hijo 2
    {
        printf();
        pid=fork();
        if (pid==-1)
        {
            printf("Error en el fork()\n");
		    exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("[HIJO]: Soy el proceso hijo y mi pid es %ld \n",(long int)getpid());
		    exit(EXIT_SUCCESS);
        }
        else //espera
        {
            
        }
        
        exit();
    }
   pid
    
    
}