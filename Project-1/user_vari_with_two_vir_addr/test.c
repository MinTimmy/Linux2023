#include <stdio.h>
#include <stdlib.h>


int main()
{
    unsigned char *buffer = malloc(sizeof(unsigned char));

    int test = 1;

    sscanf(test, "%hhx", buffer);

}
// int main()
// {
//     int *a = NULL;
//     int *b = NULL;
//     int *ptr_ptr[2];

//     int **pp = ptr_ptr;
//     // printf("%d", *(*pp+1));
//     return 0;
// }