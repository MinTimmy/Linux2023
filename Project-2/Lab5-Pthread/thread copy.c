#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>


// #include "my_header.h"
#define TOTAL_ITERATION_NUM  10000000
#define NUM_OF_PRIORITIES_TESTED  40
#define NUM_OF_ROUND 50
#define NUM_OF_REAL_TIME 9
#define NUM_OF_PARTNER 10
#define NUM_OF_NORMAL 39

struct Data
{
    int				        on_rq;
	int				        prio;
	int				        static_prio;
	int				        normal_prio;
	unsigned int			rt_priority;
    uint64_t                vruntime;
};
struct Thread_Param
{
    int                     id;
    char                    policy_name[20];
    unsigned long           spending_time_average;
    int				        my_fixed_prio;
	int				        my_fixed_static_prio;
	int				        my_fixed_normal_prio;
	unsigned int			my_fixed_rt_priority;
    uint64_t                my_fixed_vruntime;
    struct Data             data;   
};


static inline unsigned long cal_spending_time(struct timeval *start, struct timeval *end)
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

static inline void my_lab(struct timeval *start, struct timeval *end)
{
    gettimeofday(start, NULL);           //begin
    for(int i = 1; i <= TOTAL_ITERATION_NUM; i++){
        // printf("\00");
        rand();
        // sleep(1);
        // count++;
    }
            
    gettimeofday(end, NULL);             //end   
}

static inline void init_param_real_time(struct sched_param **param, struct Thread_Param **thread_param, pthread_attr_t **attr, int policy, int num_of_thread)
{
    for (int i = 0; i < num_of_thread; i++){
        attr[i] = malloc(sizeof(pthread_attr_t));
        param[i] = malloc(sizeof(struct sched_param));
        pthread_attr_init(attr[i]);
        
        param[i]->sched_priority = (i + 1) * 10;

        pthread_attr_setschedpolicy(attr[i], policy);
        pthread_attr_setschedparam(attr[i], param[i]);
        pthread_attr_setinheritsched(attr[i],PTHREAD_EXPLICIT_SCHED);

        thread_param[i] = malloc(sizeof(struct Thread_Param));
        thread_param[i]->id = i;
        check_policy(policy, thread_param[i]->policy_name);
        thread_param[i]->spending_time_average = 0;
    }
}

int my_set_process_priority(struct Thread_Param *thread_param, struct Data *data)
{
    unsigned long res = syscall(339, thread_param->my_fixed_prio, thread_param->my_fixed_static_prio, thread_param->my_fixed_rt_priority, thread_param->my_fixed_vruntime, (void*)data);
    if (!res){
        return 1;
    }
    else {
        printf("syscall error\n");
        return 0;
    }
    
}
void Thread(void *arg)
{
    sleep(1);
    struct Thread_Param *thread_param = (struct Thread_Param*) arg;
    int                 policy;
    struct sched_param  param;
    struct timeval      start, end;
    unsigned long       spending_time;

    pthread_getschedparam(pthread_self(),&policy,&param);


    my_lab(&start, &end);

    spending_time = cal_spending_time(&start, &end);
    thread_param->spending_time_average = thread_param->spending_time_average ? 
        (thread_param->spending_time_average + spending_time) / 2 : 
        spending_time;

    thread_param->my_fixed_rt_priority = param.sched_priority;
    // printf("policy: %s, priority: %d, spending_time: %ld\n", thread_param->policy_name, param.sched_priority, spending_time);
    // printf("Pthread %d exit\n", thread_param->id);
}

void Thread_Syscall(void *arg)
{
    sleep(1);
    struct Thread_Param *thread_param = (struct Thread_Param*) arg;
    // struct Data *data = (struct Data*)malloc(sizeof(struct Data));
    int                 policy;
    struct sched_param  param;
    struct timeval      start, end;
    unsigned long       spending_time;

    my_set_process_priority(thread_param, &thread_param->data);

    my_lab(&start, &end);

    spending_time = cal_spending_time(&start, &end);
    thread_param->spending_time_average = thread_param->spending_time_average ? 
        (thread_param->spending_time_average + spending_time) / 2 : 
        spending_time;

    pthread_getschedparam(pthread_self(),&policy,&param);

    memset(&thread_param->data, 0, sizeof(struct Data));

    my_set_process_priority(thread_param, &thread_param->data);
}

void Thread_Partner()
{
    while(1){
        rand();
    }
}

static inline void __pthread_create(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, void * thread_func, int num_of_thread)
{
    for (int i = 0; i < num_of_thread; i++){
        ppid[i] = malloc(sizeof(pthread_t));
        // pthread_create(ppid[i],attr[i],(void *)Thread,thread_param[i]);
        if (thread_param == NULL && attr != NULL)
            pthread_create(ppid[i],attr[i],thread_func, NULL);
        
        else if (thread_param != NULL && attr == NULL)
            pthread_create(ppid[i],NULL ,thread_func, thread_param[i]);
        
        else if (thread_param == NULL && attr == NULL)
            pthread_create(ppid[i],NULL,thread_func, NULL);

        else
            pthread_create(ppid[i],attr[i],thread_func, thread_param[i]);
    }
}
static inline void __pthread_join(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, int num_of_thread)
{
    for (int i = 0; i < num_of_thread; i++){
        pthread_join(*ppid[i],NULL);
    }
}
static inline void __pthread_attr_destory(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, int num_of_thread)
{
    for (int i = 0; i < num_of_thread; i++){
        pthread_attr_destroy(attr[i]);
    }
}

static inline void check_root()
{
    int i = -1;
    i = getuid();
    if(i == 0)
        printf("The current user is root\n");
    else
        printf("The current user is not root\n");
}
static inline void lab_real_time()
{
    pthread_t               *ppid_real_time[NUM_OF_REAL_TIME];
    pthread_t               *ppid_partner[NUM_OF_PARTNER];
    pthread_t               ppid_normal;

    struct sched_param      *param_real_time[NUM_OF_REAL_TIME];

    struct Thread_Param     *thread_param_real_time[NUM_OF_REAL_TIME];
    struct Thread_Param     *thread_param_normal = malloc(sizeof(struct Thread_Param));

    pthread_attr_t          *attr_real_time[NUM_OF_REAL_TIME];
    // pthread_attr_t          *attr_normal[NUM_OF_NORMAL];

    check_root();

    
    init_param_real_time(param_real_time, thread_param_real_time, attr_real_time, SCHED_FIFO, NUM_OF_REAL_TIME);


    __pthread_create(NULL, NULL, ppid_partner, (void *)Thread_Partner, NUM_OF_PARTNER);
    __pthread_create(thread_param_real_time, attr_real_time, ppid_real_time, (void *)Thread, NUM_OF_REAL_TIME);

    strcpy(thread_param_normal->policy_name, "SCHED_OTHER\0");
    pthread_create(&ppid_normal,NULL,(void *)Thread, thread_param_normal);


    __pthread_join(thread_param_real_time, attr_real_time, ppid_real_time, NUM_OF_REAL_TIME);
    pthread_join(ppid_normal,NULL);

    __pthread_attr_destory(thread_param_real_time, attr_real_time, ppid_real_time, NUM_OF_REAL_TIME);

    for (int i = 0; i < NUM_OF_REAL_TIME; i++){
        printf("policy: %s, priority: %d, spending_time: %ld\n", thread_param_real_time[i]->policy_name, thread_param_real_time[i]->my_fixed_rt_priority, thread_param_real_time[i]->spending_time_average);
    }
    printf("policy: %s, priority: %d, spending_time: %ld\n", thread_param_normal->policy_name, thread_param_normal->my_fixed_rt_priority, thread_param_normal->spending_time_average);

}


static inline void lab_normal()
{
    pthread_t               *ppid_normal[NUM_OF_NORMAL];
    pthread_t               *ppid_partner[NUM_OF_PARTNER];

    struct Thread_Param     *thread_param_normal[NUM_OF_NORMAL];


    check_root();

    __pthread_create(NULL, NULL, ppid_partner, (void *)Thread_Partner, NUM_OF_PARTNER);

    for (int i = 0; i < NUM_OF_NORMAL; i++){
        // ppid_normal[i] = malloc(sizeof(pthread_t));
        thread_param_normal[i] = malloc(sizeof(struct Thread_Param));

        thread_param_normal[i]->spending_time_average   = 0;
        thread_param_normal[i]->my_fixed_prio           = 0;
        thread_param_normal[i]->my_fixed_static_prio    = 101 + i;
        thread_param_normal[i]->my_fixed_normal_prio    = 0;
        thread_param_normal[i]->my_fixed_rt_priority    = 0;
        thread_param_normal[i]->my_fixed_vruntime       = 0;   
    }

    __pthread_create(thread_param_normal, NULL, ppid_normal, (void *)Thread_Syscall, NUM_OF_NORMAL);

    __pthread_join(NULL, NULL, ppid_normal, NUM_OF_NORMAL);

    for (int i = 0; i < NUM_OF_NORMAL; i++){
        printf("static_prio: %d, spending_time_average: %ld\n", thread_param_normal[i]->data.static_prio, thread_param_normal[i]->spending_time_average);
    }
}


int main()
{
    // lab_real_time();
    // lab_normal();
    lab_real_time_syscall();
    
    return 0;
}