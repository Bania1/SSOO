#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*

*/

int main()
{
    pid_t pid, flag;
    int status;

    pid=fork(); // hijo a
    if (pid == -1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("[HIJO A] --> pid %ld y padre [a.out] --> pid %ld\n", (long int)getpid(), (long int)getppid());
        
        pid=fork(); //hijo b
        if (pid == -1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("[HIJO B] --> pid %ld y padre [A] --> pid %ld\n", (long int)getpid(), (long int)getppid());
            exit(EXIT_SUCCESS);
        }


        pid=fork(); //hijo c
        if (pid == -1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("[HIJO C] --> pid %ld y padre [A] --> pid %ld\n", (long int)getpid(), (long int)getppid());
            exit(EXIT_SUCCESS);
        }
        else //espera para a
        {
            while (flag = wait(&status) > 0)
            {
                if (WIFEXITED(status)) {
                        printf("child exited, status=%d\n", WEXITSTATUS(status));


                    } else if (WIFSIGNALED(status)) {
                        printf("child killed (signal %d)\n", WTERMSIG(status));


                    } else if (WIFSTOPPED(status)) {
                        printf("child stopped (signal %d)\n", WSTOPSIG(status));


                #ifdef WIFCONTINUED     /* Not all implementations support this */
                    } else if (WIFCONTINUED(status)) {
                        printf("child continued\n");
                #endif
                    } else {    /* Non-standard case -- may never happen */
                        printf("Unexpected status (0x%x)\n", status);
                    }
            }
            exit(EXIT_SUCCESS);
        }
        
        exit(EXIT_SUCCESS);
    }
    
    pid=fork(); //hijo d
    if (pid == -1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("[HIJO D] --> pid %ld y padre [a.out] --> pid %ld\n", (long int)getpid(), (long int)getppid());

        pid=fork(); //hijo e
        if (pid == -1)
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("[HIJO E] --> pid %ld y padre [D] --> pid %ld\n", (long int)getpid(), (long int)getppid());
            
            pid=fork(); //hijo f
            if (pid == -1)
            {
                printf("Error\n");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                printf("[HIJO F] --> pid %ld y padre [E] --> pid %ld\n", (long int)getpid(), (long int)getppid());
                exit(EXIT_SUCCESS);
            }
            else //espera a e
            {
                while (flag = wait(&status) > 0)
                {
                    if (WIFEXITED(status)) {
                            printf("child exited, status=%d\n", WEXITSTATUS(status));


                        } else if (WIFSIGNALED(status)) {
                            printf("child killed (signal %d)\n", WTERMSIG(status));


                        } else if (WIFSTOPPED(status)) {
                            printf("child stopped (signal %d)\n", WSTOPSIG(status));


                    #ifdef WIFCONTINUED     /* Not all implementations support this */
                        } else if (WIFCONTINUED(status)) {
                            printf("child continued\n");
                    #endif
                        } else {    /* Non-standard case -- may never happen */
                            printf("Unexpected status (0x%x)\n", status);
                        }
                }
                exit(EXIT_SUCCESS);
            }
            
            exit(EXIT_SUCCESS);
        }
        else //espera a d
        {
            while (flag = wait(&status) > 0)
            {
                if (WIFEXITED(status)) {
                        printf("child exited, status=%d\n", WEXITSTATUS(status));


                    } else if (WIFSIGNALED(status)) {
                        printf("child killed (signal %d)\n", WTERMSIG(status));


                    } else if (WIFSTOPPED(status)) {
                        printf("child stopped (signal %d)\n", WSTOPSIG(status));


                #ifdef WIFCONTINUED     /* Not all implementations support this */
                    } else if (WIFCONTINUED(status)) {
                        printf("child continued\n");
                #endif
                    } else {    /* Non-standard case -- may never happen */
                        printf("Unexpected status (0x%x)\n", status);
                    }
            }
            exit(EXIT_SUCCESS);
        }
        

        exit(EXIT_SUCCESS);
    }
    
    pid=fork(); //hijo g
    if (pid == -1)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("[HIJO G] --> pid %ld y padre [a.out] --> pid %ld\n", (long int)getpid(), (long int)getppid());
        exit(EXIT_SUCCESS);
    }
    else //espera para ejecutable
    {
        while (flag = wait(&status) > 0)
        {
            if (WIFEXITED(status)) {
                    printf("child exited, status=%d\n", WEXITSTATUS(status));


                } else if (WIFSIGNALED(status)) {
                    printf("child killed (signal %d)\n", WTERMSIG(status));


                } else if (WIFSTOPPED(status)) {
                    printf("child stopped (signal %d)\n", WSTOPSIG(status));


            #ifdef WIFCONTINUED     /* Not all implementations support this */
                } else if (WIFCONTINUED(status)) {
                    printf("child continued\n");
            #endif
                } else {    /* Non-standard case -- may never happen */
                    printf("Unexpected status (0x%x)\n", status);
                }
        }
        exit(EXIT_SUCCESS);
    }
    

}