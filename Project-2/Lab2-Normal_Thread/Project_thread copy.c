#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

#define TOTAL_ITERATION_NUM  10000000
#define NUM_OF_PRIORITIES_TESTED  40
#define NUM_OF_ROUND 50
#define RESULT_SAVE_FILE "output_thread.txt"
unsigned long spending_time_average[NUM_OF_PRIORITIES_TESTED] = {0};

struct Data
{
    int	                    on_rq;
	int		                prio;
	int		                static_prio;
	int		                normal_prio;
	unsigned int            rt_priority;
};

struct Index_Priority
{
    int                     index;
    int                     priority;
    // struct timeval          *start;
    // struct timeval          *end;
};


int my_set_process_priority(int priority)
{
    // priority = 0;
    // printf("priority: %d\n", priority);
    struct Data *data = (struct Data*)malloc(sizeof(struct Data));
    unsigned long res = syscall(339, priority, (void*)data);
    if (!res){
        // printf("on_rq:          %d\n", data->on_rq);
        // printf("prio:           %d\n", data->prio);
        // printf("static_prio:    %d\n", data->static_prio);
        // printf("normal_prio:    %d\n", data->normal_prio);
        // printf("rt_priority:    %d\n", data->rt_priority);
        return 1;
    }
    return 0;
}

unsigned long cal_spending_time(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec * 1000000 + end->tv_usec) - (start->tv_sec * 1000000 + start->tv_usec);
}

// For thread running
void *cal_timer(void *arg)
{
    struct Index_Priority *data = (struct Index_Priority*) arg;
    struct timeval      start, end;

    printf("index: %d\n", data->index);

    if (data->index)
        if(!my_set_process_priority(data->priority))
            printf("Cannot set priority %d\n", data->priority);  

    gettimeofday(&start, NULL);           //begin
    for(int i=1;i<=TOTAL_ITERATION_NUM;i++)
        rand();
        
    gettimeofday(&end, NULL);             //end

    unsigned long *result = malloc(sizeof(unsigned long));
    *result = cal_spending_time(&start, &end);
    // printf("Exit\n");
    pthread_exit((void *) result);
}
void run_lab()
{
    int                     index=0;
    int                     priority, i;
    pthread_t               p_t[NUM_OF_PRIORITIES_TESTED];
    struct Index_Priority   *ip;
    void                    *ret;
    unsigned long           *result;

    
    /*================================================================================*/

    // Create the thread for function cal_timer()
    for(index = 0, priority = 101-1;priority<=139;++priority,++index)
    {
        ip = (struct Index_Priority*)malloc(sizeof(struct Index_Priority));
        ip->index =          index;
        ip->priority =       priority;
        // printf("ip->priority: %d\n", ip->priority);

        pthread_create(&p_t[index],NULL,cal_timer,(void*) ip);                   
    }

    // Get the result of every thread
    for (i = 0; i < NUM_OF_PRIORITIES_TESTED; i++){
        // printf("i = %d\n", i);
        pthread_join(p_t[i], &ret);
        result = (unsigned long*)ret;
        spending_time_average[i] = spending_time_average[i] ? (*result + spending_time_average[i]) / 2 : *result;
        free(result);
    }

    /*================================================================================*/

    // for(i = 0; i <= NUM_OF_PRIORITIES_TESTED - 1; i++){
    //     if (!i)
    //         printf(
    //             "The process spent %ld uses to execute when priority is not adjusted.\n", 
    //             spending_time_average[i]
    //         );
    //     else
    //         printf(
    //             "The process spent %ld uses to execute when priority is %d.\n", 
    //             spending_time_average[i],
    //             i+100
    //         );
    // }
        
}

int main()
{
    memset(spending_time_average, 0, NUM_OF_PRIORITIES_TESTED * sizeof(unsigned long));
    FILE *fptr;
    for (int i = 0; i < NUM_OF_ROUND; i++){
        printf("Round %d\n", i);
        run_lab();
        // break;
        for (int j = 0; j < NUM_OF_PRIORITIES_TESTED; j++){
            if (!j){
                printf(
                    "The process spent %ld uses to execute when priority is not adjusted.\n", 
                    spending_time_average[j]
                );
                fptr = fopen(RESULT_SAVE_FILE, "w");
            }   
            else{
                printf(
                    "The process spent %ld uses to execute when priority is %d.\n", 
                    spending_time_average[j],
                    j+100
                );
                fptr = fopen(RESULT_SAVE_FILE, "a");
                fprintf(fptr, "\n");
            }
            fprintf(fptr, "%ld", spending_time_average[j]);
            fclose(fptr);
        }
        printf("\n=======================================================\n");
    }
}