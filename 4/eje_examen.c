#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void bunda(int signal)
{
    if (signal==SIGUSR1)
    {
        printf("\nHe recibido la señal SIGUSR1 con cod. %d\n", signal);
    }
    else if (signal==SIGINT)
    {
        printf("\nHe recibido la señal SIGINT con cod. %d\n", signal);
        exit(EXIT_SUCCESS);
    }
    else if (signal==SIGKILL)
    {
        printf("\nHe recibido la señal SIGKILL con cod. %d\n", signal);
        exit(EXIT_SUCCESS);
    }
    
}

int main()
{
    pid_t pid, flag;
    int status;

    printf("Soy el niggaPadre con pid %ld\n", (long int)getpid());

    pid=fork();
    if (pid==-1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid==0)
    {
        printf("Soy el hijo con pid %ld y mi Padre tiene pid %ld \n", (long int)getpid(), (long int)getppid());
        
        signal(SIGUSR1, &bunda);
        signal(SIGINT, &bunda);
        signal(SIGKILL, &bunda);
        while(1);
    }
    else
    {
        sleep(1);
            
        kill(pid, SIGUSR1);
        sleep(1);

        kill(pid, SIGINT);
        sleep(1);

        kill(pid, SIGKILL);
        
        while((flag=wait(&status))>0){
            

            if(WIFEXITED(status)){
		        printf("hijo %ld finalizado con status %d\n",(long int)flag,WEXITSTATUS(status));
		    }
		    else if(WIFSIGNALED(status)){
		        printf("hijo %ld finalizado tras recibir una senal con status %d\n",(long int)flag,WTERMSIG(status));
		    }
		    else if(WIFSTOPPED(status)){
		        printf("hijo %ld parado con status %d\n",(long int)flag,WSTOPSIG(status));
		    }
		    else if(WIFCONTINUED(status)){
		        printf("hijo %ld reanudado\n",(long int)flag);
		    }

		}
		if(flag==(pid_t)-1 && errno==ECHILD){ 
		    printf("Valor del errno= %d, definido como %s\n",errno,strerror(errno));
		}
		else{
		    printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal\n");
		    exit(EXIT_FAILURE);
		} 

        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}