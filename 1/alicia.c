#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

//Fichero para aprender con alicia
int main(){

    pid_t pid,flag;
    int status;
    printf("Soy el padre con pid %ld\n", (long int)getpid());

    pid=fork();

    if(pid==-1){
        printf("error 1");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        printf("HIJO 1: Soy el hijo 1 con pid %ld y con padre %ld\n", (long int)getpid(), (long int)getppid());
        exit(EXIT_SUCCESS);
    }
    
    pid=fork();

    if(pid==-1){
        printf("error 2");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        printf("HIJO 2: Soy el hijo 2 con pid %ld y con padre %ld\n", (long int)getpid(), (long int)getppid());
        pid=fork();
        if(pid==-1){
            printf("error 3");
            exit(EXIT_FAILURE);
        }
        else if(pid==0){
            printf("HIJO 3: Soy el hijo 3 con pid %ld y con padre %ld\n", (long int)getpid(), (long int)getppid());
            exit(EXIT_SUCCESS);
        }
        else{
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

        }

    }
    else{

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
    }
}
