#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void funcionManejadora(int signal)
{
	printf("He recibido la señal SIGUSR1 con codigo %d.\n",signal );
	
}

void funcionManejadora2(int signal)
{
	printf("He recibido la señal ctrl+c con codigo %d.\n",signal );
	
}

void funcionManejadora3(int signal)
{
	printf("He recibido la señal SIGKILL con codigo %d.\n",signal );
	exit(EXIT_SUCCESS);
}

int main()
{

	pid_t pid, flag;
	int status;

	pid = fork(); //pid puede contener un -1 (error) o bien dos valores posibles (cero o distinto de cero)

	if(pid == -1) //ERROR
	{
		printf("Error en el fork()\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) //HIJO
	{
		printf("[HIJO]: Soy el proceso hijo %ld y mi padre es %ld.\n",(long int)getpid(),(long int)getppid());
		signal(SIGUSR1,&funcionManejadora);
		signal(SIGINT,&funcionManejadora2);
		signal(SIGKILL,&funcionManejadora3);

		//Bucle infinito
		while(1);

	} 
	else //PADRE
	{
		printf("[PADRE]: Soy el proceso %ld y mi padre es %ld.\n",(long int)getpid(),(long int)getppid());

		sleep(1);
		kill(pid,SIGUSR1);
		sleep(1);
		kill(pid,SIGINT);
		sleep(1);
		kill(pid,SIGKILL);

		//espera bloqueante
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


}