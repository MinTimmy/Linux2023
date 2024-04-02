#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

#include "header.h"

int my_set_process_priority(int my_fixed_priority, int my_fixed_static_prio, unsigned int my_fixed_rt_priority, uint64_t my_fixed_vruntime)
{
    // my_fixed_priority = 110;
    // my_fixed_static_prio = 0;
    // my_fixed_rt_priority = 2;
    // uint64_t my_fixed_vruntime = 10000000 / priority;
    // uint64_t my_fixed_vruntime = 0;
    
    struct Data *data = (struct Data*)malloc(sizeof(struct Data));
    unsigned long res = syscall(339, my_fixed_priority, my_fixed_static_prio, my_fixed_rt_priority, my_fixed_vruntime, (void*)data);
    if (!res){
        // printf("on_rq:          %d\n", data->on_rq);
        // printf("prio:           %d\n", data->prio);
        // printf("static_prio:    %d\n", data->static_prio);
        // printf("normal_prio:    %d\n", data->normal_prio);
        // printf("rt_priority:    %d\n", data->rt_priority);
        // printf("vruntime:    %ld\n", data->vruntime);
        return 1;
    }
    else {
        printf("syscall error\n");
        return 0;
    }
    
}
unsigned long cal_spending_time(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec * 1000000 + end->tv_usec) - (start->tv_sec * 1000000 + start->tv_usec);
}

static inline void lab_for_function()
{
    for(int i = 1; i <= TOTAL_ITERATION_NUM; i++){
        // count++;
        rand();
        // sleep(1);
        // printf("\00");
    }
}

void run_lab(unsigned long *spending_time_average)
{
    int              index=0;
    int              priority,i;
    struct timeval   start[NUM_OF_PRIORITIES_TESTED], end[NUM_OF_PRIORITIES_TESTED];
    
    gettimeofday(&start[index], NULL);           //begin
   
    lab_for_function();

    gettimeofday(&end[index], NULL);             //end 

    /*================================================================================*/

    for(index=1, priority=101;priority<=139;++priority,++index)
    {
        // printf("\n==========================================\n");
        printf("index: %d\n", index);
        if(!my_set_process_priority(priority, 0, 0, 0))
            printf("Cannot set priority %d\n", priority);  

        gettimeofday(&start[index], NULL);           //begin
       
        lab_for_function();
            
        gettimeofday(&end[index], NULL);             //end                       
        // my_set_process_priority(0);
    }

    /*================================================================================*/

    unsigned long spending_time; 
    for(i = 0; i <= NUM_OF_PRIORITIES_TESTED - 1; i++){
        spending_time = cal_spending_time(&start[i], &end[i]);
        spending_time_average[i] = spending_time_average[i] ? (spending_time + spending_time_average[i]) / 2 : spending_time;
        // if (!i)
        //     printf(
        //         "The process spent %ld uses to execute when priority is not adjusted.\n", 
        //         spending_time
        //     );
        // else
        //     printf(
        //         "The process spent %ld uses to execute when priority is %d.\n", 
        //         spending_time,
        //         i+100
        //     );
    }
        
}



void run_lab_with_count_content_switching(unsigned long *spending_time_average, uint64_t *count_content_switching)
{
    int              index=0;
    int              priority,i;
    struct timeval   start[NUM_OF_PRIORITIES_TESTED], end[NUM_OF_PRIORITIES_TESTED];
    uint64_t         count_content_switching_[NUM_OF_PRIORITIES_TESTED];
    syscall(341); 
    gettimeofday(&start[index], NULL);           //begin
   
    lab_for_function();

    gettimeofday(&end[index], NULL);             //end 
    count_content_switching_[0] = syscall(342);

    /*================================================================================*/

    for(index=1, priority=101;priority<=139;++priority,++index)
    {
        // printf("\n==========================================\n");
        printf("index: %d\n", index);
        if(!my_set_process_priority(priority, 0, 0, 0))
            printf("Cannot set priority %d\n", priority);
        syscall(341);

        gettimeofday(&start[index], NULL);           //begin
       
        lab_for_function();
            
        gettimeofday(&end[index], NULL);             //end                       
        count_content_switching_[index] = syscall(342);
        // my_set_process_priority(0);
    }

    /*================================================================================*/

    unsigned long spending_time; 
    for(i = 0; i <= NUM_OF_PRIORITIES_TESTED - 1; i++){
        spending_time = cal_spending_time(&start[i], &end[i]);
        spending_time_average[i]    = spending_time_average[i]      ? (spending_time                    + spending_time_average[i])     / 2 : spending_time;
        count_content_switching[i]  = count_content_switching[i]    ? (count_content_switching_[i]      + count_content_switching[i])   / 2 : count_content_switching_[i];
        // if (!i)
        //     printf(
        //         "The process spent %ld uses to execute when priority is not adjusted.\n", 
        //         spending_time
        //     );
        // else
        //     printf(
        //         "The process spent %ld uses to execute when priority is %d.\n", 
        //         spending_time,
        //         i+100
        //     );
    }
        
}