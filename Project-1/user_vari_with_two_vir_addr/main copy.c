#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

#define MY_PAGE_NUM 3




int main()
{
    struct Data *data = malloc(sizeof(struct Data));
    // struct Data *kernel_data = malloc(sizeof(struct Data)* MY_PAGE_NUM);
    // struct Data kernel_data[MY_PAGE_NUM];
    // memset(data, 0, sizeof(struct Data) * MY_PAGE_NUM);
    memset(data, 0, sizeof(struct Data));
    
    unsigned long target = 20001227;

    data->user.value = target;
    data->user.virtual_address = (unsigned long)&target;

    unsigned long res = 0;
    res = syscall(340, (void *)data);

    printf("res: %ld\n", res);
    if (!res){
        printf("kernel\n");
        show_page_table_value(&data->kernel);
        printf("user\n");
        show_page_table_value(&data->user);
    }
    return 0;
}

