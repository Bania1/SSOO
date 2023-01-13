#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int contarFich(char *nf)
{
    FILE *fich = fopen(nf, "r"); 
    if (fich==NULL)
    {
        printf("Error \n");
        exit(EXIT_FAILURE);
    }
    int cont=0;
    char linea[200];
    while (fgets(linea, 200, fich))
    {
        cont++;
    }
    fclose(fich);
    return(cont);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Error, es necesario insertar los ficheros\n");
        exit(EXIT_FAILURE);
    }
    char *nf1=argv[1];
    char *nf2=argv[2];
    char *nf3=argv[3];

    pid_t pid, flag;
    int status;
    int cont=0;

    for (int i = 0; i < 3; i++)
    {
        pid=fork();
        if (pid==-1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid==0)
        {
            if (i==0)
            {
                cont += contarFich(nf1);
                exit(cont);
            }
            else if (i==1)
            {
                cont += contarFich(nf2);
                exit(cont);
            }
            else if (i==2)
            {
                cont += contarFich(nf3);
                exit(cont);
            }
        }
    }
    while((flag=wait(&status))>0)
    {
        cont+=WEXITSTATUS(status); //aqui obtenemos lo que devuelven los procesos

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

    printf("la suma de registros es %d\n",cont); //la suma de todo

    exit(EXIT_SUCCESS);

    return 0;
}
