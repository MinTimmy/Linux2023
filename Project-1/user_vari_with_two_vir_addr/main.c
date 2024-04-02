#define _GNU_SOURCE

#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

#define MY_PAGE_NUM 3

extern void *func1(void *);
extern void *func2(void *);
extern int main();


unsigned long global_variable = 1;
unsigned long uninit_global_variable;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


static __thread  sdata  tx ;  //thread local variable


void sub_show(char *name, unsigned long virtual_address)
{
    struct Data *data = malloc(sizeof(struct Data));
    memset(data, 0, sizeof(struct Data));

    data->user.virtual_address  = virtual_address;
    data->user.value            = *(unsigned long*)virtual_address;

    syscall(340, (void *)data);

    printf("%-30s 0x%016lx (0x%016lx), 0x%016lx (0x%016lx)\n", 
        name, 
        data->user.virtual_address, data->user.physical_address,
        data->kernel.virtual_address, data->kernel.physical_address
    );
    // show_page_table_value(&data->user);
    // show_page_table_value(&data->kernel);
    free(data);
}

void show(int pid, void *stack_addr)
{
    char name[30] = "";
    printf("thread pid: %d\n", pid);

    strcpy(name,"global variable") ;
    sub_show(name, (unsigned long)&global_variable);
   
    strcpy(name,"uninit global variable") ;
    sub_show(name, (unsigned long)&uninit_global_variable);

    strcpy(name,"function show") ;
    sub_show(name, (unsigned long)show);
    
    strcpy(name,"func1") ;
    sub_show(name, (unsigned long)func1);

    strcpy(name,"func2") ;
    sub_show(name, (unsigned long)func2);

    strcpy(name,"main") ;
    sub_show(name, (unsigned long)main);

    strcpy(name,"printf") ;
    sub_show(name, (unsigned long)printf);

    int *heap_addr = malloc(sizeof(int));
    strcpy(name,"heap") ;
    sub_show(name, (unsigned long)heap_addr);
    free(heap_addr);


    border();
    pthread_mutex_unlock(&mutex);

}

void *thread_preprocess()
{
    pthread_attr_t attr;
    void *stack_addr;
    size_t stack_size;

    
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    // print_info(stack_addr);

    // struct Page_Table_Value *ptv;
    printf("stack_addr: %p\n", stack_addr);
    printf("stack_size: %lu bytes\n", stack_size);
    
    
    return stack_addr;
}

void *func1(void *arg)
{
    pthread_mutex_lock(&mutex); 

    char *p = (char*) arg ;
    int pid  ;
    pid =  syscall( __NR_gettid );
    tx.id = pid ;
    strcpy(tx.name,p) ;
       

    printf("I am thread with ID %d executing func1().\n",pid);
    void *stack_addr = thread_preprocess();
    show(pid, stack_addr);
    while(1)
    {
        // printf("(%d)(%s)\n",tx.id,tx.name) ;
        sleep(1) ;
    }
}

void *func2(void *arg)
{
    pthread_mutex_lock(&mutex); 
    
    char *p = (char*) arg ;
    int pid  ;
    pid =  syscall( __NR_gettid );
    tx.id = pid ;
    strcpy(tx.name,p) ;

    printf("I am thread with ID %d executing func2().\n",pid);
    
    void *stack_addr = thread_preprocess();
    show(pid, stack_addr);
    while(1)
    {
        // printf("(%d)(%s)\n",tx.id,tx.name) ;
        sleep(2) ;
    }
}
int main()
{
    pthread_t id[2];
    char p[2][16] ;
    strcpy(p[0],"Thread1") ;
    pthread_create(&id[0],NULL,func1,(void *)p[0]);
    strcpy(p[1],"Thread2") ;
    pthread_create(&id[1],NULL,func2,(void *)p[1] );


    int pid;
    pid =  syscall( __NR_gettid );
    tx.id = pid ;
    strcpy(tx.name,"MAIN") ;


    pthread_mutex_lock(&mutex);    
    printf("I am main thread with ID %d.\n", pid);
    show(pid, NULL);
    while(1)
    {
        //printf("(%d)(%s)\n",tx.id,tx.name) ;
        sleep(5) ;
    }
    return 0;
}

