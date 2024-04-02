#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <linux/types.h>


#include "my_header.h"
#define TOTAL_ITERATION_NUM  100000000
#define NUM_OF_PRIORITIES_TESTED  40
#define NUM_OF_ROUND 50

unsigned long spending_time_average[NUM_OF_PRIORITIES_TESTED] = {0};


int my_set_process_priority(int priority)
{
    priority = 0;
    int my_fixed_static_prio = priority;
    // uint64_t my_fixed_vruntime = 10000000 / priority;
    uint64_t my_fixed_vruntime = 0;
    struct Data *data = (struct Data*)malloc(sizeof(struct Data));
    unsigned long res = syscall(339, priority, my_fixed_static_prio, my_fixed_vruntime, (void*)data);
    // printf("res: %ld\n", res);
    // int res = 0;
    if (!res){
        // if(priority){
        //     printf("(Before timer my priority)     ");
        // }
        // else{
        //     printf("(After timer reset my priority)");
        // }
        printf("on_rq:          %d\n", data->on_rq);
        printf("prio:           %d\n", data->prio);
        printf("static_prio:    %d\n", data->static_prio);
        printf("normal_prio:    %d\n", data->normal_prio);
        printf("rt_priority:    %d\n", data->rt_priority);
        printf("vruntime:    %ld\n", data->vruntime);
        return 1;
    }
    return 0;
}
unsigned long cal_spending_time(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec * 1000000 + end->tv_usec) - (start->tv_sec * 1000000 + start->tv_usec);
}
void run_lab()
{
    int              index=0;
    int              priority,i;
    struct timeval   start[NUM_OF_PRIORITIES_TESTED], end[NUM_OF_PRIORITIES_TESTED];
    int count = 0;
    
    gettimeofday(&start[index], NULL);           //begin
    for(i=1;i<=TOTAL_ITERATION_NUM;i++){
        // count++;
        rand();
        // sleep(1);
        // printf("\00");
    }
        
    gettimeofday(&end[index], NULL);             //end 

    /*================================================================================*/

    for(index=1, priority=101;priority<=139;++priority,++index)
    {
        // printf("\n==========================================\n");
        printf("index: %d\n", index);
        if(!my_set_process_priority(priority))
            printf("Cannot set priority %d\n", priority);  

        gettimeofday(&start[index], NULL);           //begin
        for(i=1;i<=TOTAL_ITERATION_NUM;i++){
            // printf("\00");
            rand();
            // sleep(1);
            // count++;
        }
            
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

int main()
{
    memset(spending_time_average, 0, NUM_OF_PRIORITIES_TESTED * sizeof(unsigned long));
    FILE *fptr;
    for (int i = 0; i < NUM_OF_ROUND; i++){
        printf("Round %d\n", i);

        run_lab();
        for (int j = 0; j < NUM_OF_PRIORITIES_TESTED; j++){
        //    char str[50];
        //    sprintf(str, "%ld", spending_time_average[j]);
            if (!j){
                // printf(
                //     "The process spent %ld uses to execute when priority is not adjusted.\n", 
                //     spending_time_average[j]
                // );
                // Open a file in writing mode
                fptr = fopen("output.txt", "w");

                // Write some text to the file
                fprintf(fptr, "%ld", spending_time_average[j]);

                // Close the file
                fclose(fptr);
            }   
            else{
                // printf(
                //     "The process spent %ld uses to execute when priority is %d.\n", 
                //     spending_time_average[j],
                //     j+100
                // );
                // Open a file in writing mode
                fptr = fopen("output.txt", "a");

                // Write some text to the file 
                fprintf(fptr, "\n");
                fprintf(fptr, "%ld", spending_time_average[j]);
               
                // Close the file
                fclose(fptr);
            }
        }
        printf("\n=======================================================\n");
    }
}
