#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>


// #include "my_header.h"
#define TOTAL_ITERATION_NUM  100000000
#define NUM_OF_PRIORITIES_TESTED  40
#define NUM_OF_ROUND 50
#define NUM_OF_THREAD 9

struct Thread_Param
{
    int id;
};
unsigned long cal_spending_time(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec * 1000000 + end->tv_usec) - (start->tv_sec * 1000000 + start->tv_usec);
}

static inline void check_policy(int id, char *name)
{
    if(id == SCHED_OTHER)
        strcpy(name, "SCHED_OTHER\0");
    if(id == SCHED_RR)
        strcpy(name, "SCHED_RR\0");
    if(id ==SCHED_FIFO)
        strcpy(name, "SCHED_FIFO\0");
}

void Thread(void *arg)
{
    sleep(1);
    // struct Thread_Param *thread_param = (struct Thread_Param*) arg;
    int                 i;
    int                 policy;
    char                policy_name[20];
    struct sched_param  param;
    struct timeval      start, end;

    pthread_getschedparam(pthread_self(),&policy,&param);
    // printf("Pthread %d: ", thread_param->id);

    check_policy(policy, policy_name);


    gettimeofday(&start, NULL);           //begin
    for(i=1;i<=TOTAL_ITERATION_NUM;i++){
        // printf("\00");
        rand();
        // sleep(1);
        // count++;
    }
            
    gettimeofday(&end, NULL);             //end   
    unsigned long spending_time = cal_spending_time(&start, &end);
    printf("policy: %s, priority: %d, spending_time: %ld\n", policy_name, param.sched_priority, spending_time);
    // printf("Pthread %d exit\n", thread_param->id);
}

int main()
{
    int i;
    i = getuid();
    if(i==0)
        printf("The current user is root\n");
    else
        printf("The current user is not root\n");

    pthread_t ppid[NUM_OF_THREAD];
    struct sched_param param;
    struct Thread_Param *thread_param[NUM_OF_THREAD];

    pthread_attr_t *attr[NUM_OF_THREAD];

    for (int i = 0; i < NUM_OF_THREAD; i++){
        attr[i] = malloc(sizeof(pthread_attr_t));
        pthread_attr_init(attr[i]);

        param.sched_priority = i;
        // pthread_attr_setschedpolicy(attr[i],SCHED_RR);
        pthread_attr_setschedpolicy(attr[i],SCHED_OTHER);
        pthread_attr_setschedparam(attr[i],&param);
        pthread_attr_setinheritsched(attr[i],PTHREAD_EXPLICIT_SCHED);

        thread_param[i] = malloc(sizeof(struct Thread_Param));
        thread_param[i]->id = i;
    }


    for (int i = 0; i < NUM_OF_THREAD; i++){
        pthread_create(&ppid[i],attr[i],(void *)Thread,thread_param[i]);
    }
    for (int i = 0; i < NUM_OF_THREAD; i++){
        pthread_join(ppid[i],NULL);
    }

    for (int i = 0; i < NUM_OF_THREAD; i++){
        pthread_attr_destroy(attr[i]);
    }
    return 0;
}