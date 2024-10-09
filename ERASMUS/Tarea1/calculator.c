#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Error: you must put three arguments\n");
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[3]);
    char operador = argv[2][0];

    switch (operador)
    {
        case '+':
            printf("%d + %d = %d\n", num1, num2, num1 + num2);
            break;
        
        case '-':
            printf("%d - %d = %d\n", num1, num2, num1 - num2);
            break;
        
        case '*':
            printf("%d * %d = %d\n", num1, num2, num1 * num2);
            break;
        
        case '/':
            if(num2 == 0)
            {
                printf("Error: division by zero\n");
                return 1;
            }
            
            printf("%d / %d = %d\n", num1, num2, num1 / num2);
            break;
        
        default:
            printf("Error: invalid operator\n");
            return 1;
    }
}