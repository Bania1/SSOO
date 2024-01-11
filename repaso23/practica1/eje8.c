#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void tratarSennal(int sig)
{
    static int cont = 0;
    cont++;

    printf("[HIJO %d] He recibido la senal SIGUSR1\n", cont);
}

int main ()
{
    pid_t pid, flag;
    int status;

    pid = fork();

    if (pid == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        signal(SIGUSR1, tratarSennal);

        while(1);
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            sleep(1);
            kill(pid, SIGUSR1);
        }
        sleep(1);
        kill(pid, SIGKILL);

        while((flag=wait(&status))>0)
        {
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
}