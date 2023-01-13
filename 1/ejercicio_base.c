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

	pid = fork(); //pid puede contener un -1 (error) o bien dos valores posibles (cero o distinto de cero)

	if(pid == -1) //ERROR
	{
		printf("Error en el fork()\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) //HIJO
	{
		printf("[HIJO]: Soy el proceso hijo y mi pid es %ld y mi padre es %ld.\n",(long int)getpid(), (long int)getppid());
		exit(EXIT_SUCCESS);
	} 
	else //PADRE
	{
		printf("[PADRE]: Soy el proceso padre y mi pid es %ld.\n",(long int)getpid());
		
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