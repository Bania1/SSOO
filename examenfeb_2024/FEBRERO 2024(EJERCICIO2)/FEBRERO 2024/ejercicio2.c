#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct myStruct
{
    pthread_mutex_t *m1,*m2;
    int *par,*impar;
    int indice;
};

void *sumar(struct myStruct *data)
{
    int num = 1+rand()%10;

    printf("Soy %lu, mi indice es %d y el numero aleatorio es %d.\n",pthread_self(),(data->indice),num);
    if(data->indice%2==0)
    {
        if(pthread_mutex_lock(data->m1)!=0)
        {
           printf("mutex_lock error.\n");
           exit(EXIT_FAILURE);
        }
        (*data->par)+=num;

        if(pthread_mutex_unlock(data->m1)!=0)
        {
           printf("mutex_lock error.\n");
           exit(EXIT_FAILURE);
        }
    }
    else
    {
        if(pthread_mutex_lock(data->m2)!=0)
        {
           printf("mutex_lock error.\n");
           exit(EXIT_FAILURE);
        }
        (*data->impar)+=num;

        if(pthread_mutex_unlock(data->m2)!=0)
        {
           printf("mutex_lock error.\n");
           exit(EXIT_FAILURE);
        }

    }
    
    int *devolver = (int*)malloc(sizeof(int));
    *devolver = num;
    pthread_exit((void*)devolver);

}

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("Debes introducir 2 argumentos.\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));

    int nhilos = atoi(argv[1]);
    int par = 0;
    int impar = 0;
    pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_t thread[nhilos];
    struct myStruct estructura[nhilos];

    for(int i = 0;i<nhilos;i++)
    {
        estructura[i].m1 = &m1;
        estructura[i].m2 = &m2;
        estructura[i].par = &par;
        estructura[i].impar = &impar;
        estructura[i].indice = i+1;
        if(pthread_create(&thread[i],NULL,(void*)sumar,&estructura[i])!=0)
        {
            printf("Error al crear el hilo.\n");
            exit(EXIT_FAILURE);
        }
    }

    int *devuelto;

    for(int i = 0;i<nhilos;i++)
    {
        if(pthread_join(thread[i],(void**)&devuelto)!=0)
        {
            printf("Error al recoger el hilo.\n");
            exit(EXIT_FAILURE);
        }

        printf("La hebra %lu ha devuelto %d.\n",thread[i],*devuelto);
    }
    

    printf("Soy el main, la varible par vale %d, y la variable impar %d.\n",(*estructura->par),(*estructura->impar));
   
    exit(EXIT_SUCCESS); 

}