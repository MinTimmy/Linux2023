#define _GNU_SOURCE

#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "header.h"

#define MY_PAGE_NUM 3

extern void *func1(void *);
extern void *func2(void *);
extern int main();


unsigned long global_variable = 1;
unsigned long uninit_global_variable;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


static __thread  sdata  tx ;  //thread local variable

void show(int pid, struct Process_Struct *process_struct)
{
    int local_variable = 1;
    // printf("thread pid: %d\n", pid);
    printf("%d\n", pid);

    process_struct->text.sample_value   = (unsigned long) main;
    process_struct->data.sample_value   = (unsigned long) &global_variable;
    process_struct->bss.sample_value    = (unsigned long) &uninit_global_variable;
    process_struct->heap.sample_value   = (unsigned long) process_struct;
    process_struct->mm.sample_value     = (unsigned long) printf;
    process_struct->stack.sample_value  = (unsigned long) &local_variable;

    show_kernel_user_address(process_struct);
    show_segment(process_struct);

    // border();
    pthread_mutex_unlock(&mutex);

}
 
void __show_kernel_user_address(char *name, unsigned long virtual_address)
{
    struct Data *data = malloc(sizeof(struct Data));
    memset(data, 0, sizeof(struct Data));
    virtual_address_to_physical_address(virtual_address, data);

    printf("%-30s 0x%016lx (0x%016lx), 0x%016lx (0x%016lx)\n", 
        name, 
        data->user.virtual_address, data->user.physical_address,
        data->kernel.virtual_address, data->kernel.physical_address
    );
    show_page_table_value(&data->user);
    // show_page_table_value(&data->kernel);
    free(data);
}

void show_kernel_user_address(struct Process_Struct *process_struct)
{
    char name[30] = "";
    strcpy(name,"global variable") ;
    __show_kernel_user_address(name, (unsigned long)&global_variable);
   
    strcpy(name,"uninit global variable") ;
    __show_kernel_user_address(name, (unsigned long)&uninit_global_variable);
    
    strcpy(name,"local variable") ;
    __show_kernel_user_address(name, (unsigned long)process_struct->stack.sample_value);

    strcpy(name,"function show") ;
    __show_kernel_user_address(name, (unsigned long)show);
    
    strcpy(name,"func1") ;
    __show_kernel_user_address(name, (unsigned long)func1);

    strcpy(name,"func2") ;
    __show_kernel_user_address(name, (unsigned long)func2);

    strcpy(name,"main") ;
    __show_kernel_user_address(name, (unsigned long)main);

    strcpy(name,"printf") ;
    __show_kernel_user_address(name, (unsigned long)printf);

    strcpy(name,"heap") ;
    __show_kernel_user_address(name, (unsigned long)process_struct);
}  



void *thread_preprocess()
{
    pthread_attr_t attr;
    void *stack_addr;
    size_t stack_size;
    
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);  
    
    return stack_addr;
}

void *func1(void *arg)
{
    pthread_mutex_lock(&mutex); 

    char *p = (char*) arg ;
    int pid  ;
    struct Process_Struct *process_struct  = malloc(sizeof(struct Process_Struct));

    pid =  syscall( __NR_gettid );
    tx.id = pid ;
    strcpy(tx.name,p) ;
       
    // printf("I am thread with ID %d executing func1().\n",pid);

    void *stack_addr = thread_preprocess();

    process_struct->stack_addr = (unsigned long) stack_addr;

    show(pid, process_struct);
    free(process_struct);

    // while(1)
    // {
    //     // printf("(%d)(%s)\n",tx.id,tx.name) ;
    //     sleep(1) ;
    // }
    return 0;
}
void *func2(void *arg)
{
    pthread_mutex_lock(&mutex); 

    char *p = (char*) arg ;
    int pid  ;
    struct Process_Struct *process_struct  = malloc(sizeof(struct Process_Struct));

    pid =  syscall( __NR_gettid );
    tx.id = pid ;
    strcpy(tx.name,p) ;
       
    // printf("I am thread with ID %d executing func2().\n",pid);

    void *stack_addr = thread_preprocess();

    process_struct->stack_addr = (unsigned long) stack_addr;

    show(pid, process_struct);
    free(process_struct);

    // while(1)
    // {
    //     // printf("(%d)(%s)\n",tx.id,tx.name) ;
    //     sleep(1) ;
    // }
    return 0;
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
    // printf("I am main thread with ID %d.\n", pid);
    struct Process_Struct *process_struct  = malloc(sizeof(struct Process_Struct));
    memset(process_struct, 0, sizeof(struct Process_Struct));

    show(pid, process_struct);
    free(process_struct);


    // while(1)
    // {
    //     //printf("(%d)(%s)\n",tx.id,tx.name) ;
    //     sleep(5) ;
    // }
        sleep(5) ;
    return 0;
}

