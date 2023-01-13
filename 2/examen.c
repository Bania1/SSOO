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
    int status, suma;

    pid=fork(); //Hijo B
    if (pid==-1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid==0)
    {
        printf("[HIJO B] ---> %ld y [PADRE A] ---> %ld\n", (long int)getpid(), (long int)getppid());
        exit(getpid()%10); //Dato b
    }
   
    pid=fork(); //Hijo c, padre a
    if (pid==-1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid==0)
    {
        printf("[HIJO C] ---> %ld y [PADRE A] ---> %ld\n", (long int)getpid(), (long int)getppid());
        
        pid=fork(); //Hijo d, padre c
        if (pid==-1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid==0)
        {
             printf("[HIJO D] ---> %ld y [PADRE C] ---> %ld\n", (long int)getpid(), (long int)getppid());
             exit(getpid()%10); //Dato d
        }
        
        pid=fork(); //Hijo e, padre c
        if (pid==-1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid==0)
        {
            printf("[HIJO E] ---> %ld y [PADRE C] ---> %ld\n", (long int)getpid(), (long int)getppid());

            pid=fork(); //Hijo f, padre e
            if (pid==-1)
            {
                printf("Error\n");
                exit(EXIT_FAILURE);
            }
            else if (pid==0)
            {
                printf("[HIJO F] ---> %ld y [PADRE E] ---> %ld\n", (long int)getpid(), (long int)getppid());
                exit(getpid()%10); //Dato f
            }
            else //Espera de f para padre e
            {
                int dato_f;
                while((flag=wait(&status))>0)
                {
                    dato_f=WEXITSTATUS(status);

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

                exit(dato_f + (getpid()%10));
            }
            
            
            exit(EXIT_SUCCESS);
        }
        else //Espera de d y e para padre c
        {
            int dato_de = 0;

            while((flag=wait(&status))>0)
            {
                dato_de += WEXITSTATUS(status);
                
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

            exit(dato_de + (getpid()%10));
         
        }
          
        exit(EXIT_SUCCESS);
    }
    else //Espera de a
    {
            int dato_bc = 0;

            printf("[PADRE] ---> %ld\n", (long int)getpid());
            while((flag=wait(&status))>0)
            {
                dato_bc += WEXITSTATUS(status);
                
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

            printf("La suma es de %d \n", dato_bc + (getpid()%10));
            exit(EXIT_SUCCESS);
    }
   
}