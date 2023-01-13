#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int contarRegistros(char *nF)
{
    FILE *fich=fopen(nF,"r");

    if(fich==NULL)
    {
        printf("error al abrir\n");
        exit(EXIT_FAILURE);
    }
    int cont=0;
    char linea[200];
    while(fgets(linea,200,fich))
    {
        cont++;
    }
    fclose(fich);
    return cont;
}

int main(int argc,char **argv)
{
    if(argc==1)
    {
        printf("error en linea de argumentos\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid, flag;
	int status;
    int cont=0;

    for(int i=0;i<argc-1;i++)
    {
        pid=fork();
        if(pid==-1)
        {
            printf("error \n");
            exit(EXIT_FAILURE);
        }
        else if(pid==0)
        {
            cont+=contarRegistros(argv[i+1]);
            exit(cont);
        }
    }
        
    while((flag=wait(&status))>0){
        cont+=WEXITSTATUS(status);
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
        printf("la suma de registros es %d\n",cont);
    exit(EXIT_SUCCESS);

}