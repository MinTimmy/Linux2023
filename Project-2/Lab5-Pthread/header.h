#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>


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
    uint64_t                count_content_switching_average;
};

// static inline unsigned long cal_spending_time(struct timeval *start, struct timeval *end);
// static inline void check_policy(int id, char *name);
// static inline void my_lab(struct timeval *start, struct timeval *end);
// static inline void init_param_real_time(struct sched_param **param, struct Thread_Param **thread_param, pthread_attr_t **attr, int policy, int num_of_thread)


int my_set_process_priority(struct Thread_Param *thread_param, struct Data *data);


void Thread(void *arg);
void Thread_Syscall(void *arg);
void Thread_Partner();

// static inline void __pthread_create(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, void * thread_func, int num_of_thread);
// static inline void __pthread_join(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, int num_of_thread);
// static inline void __pthread_attr_destory(struct Thread_Param **thread_param, pthread_attr_t **attr, pthread_t **ppid, int num_of_thread);
// static inline void check_root();


int lab_real_time(int policy);
int lab_normal();
