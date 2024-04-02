#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

#include "./../src/header.h"

#define RESULT_SAVE_FILE "output_thread.txt"

// for the thread parameter
struct Index_Priority
{
    int                     index;
    int                     my_fixed_static_prio;
    // struct timeval          *start;
    // struct timeval          *end;
};


// For thread running
void *cal_timer(void *arg)
{
    struct Index_Priority *data = (struct Index_Priority*) arg;
    struct timeval      start, end;

    printf("index: %d\n", data->index);

    if (data->index)
        if(!my_set_process_priority(0, data->my_fixed_static_prio, 0, 0))
            printf("Cannot set priority %d\n", data->my_fixed_static_prio);  

    gettimeofday(&start, NULL);           //begin
    for(int i=1;i<=TOTAL_ITERATION_NUM;i++)
        rand();
        
    gettimeofday(&end, NULL);             //end

    unsigned long *result = malloc(sizeof(unsigned long));
    *result = cal_spending_time(&start, &end);
    // printf("Exit\n");
    pthread_exit((void *) result);
}
void run_lab_thread(unsigned long *spending_time_average)
{
    int                     index=0;
    int                     my_fixed_static_prio, i;
    pthread_t               p_t[NUM_OF_PRIORITIES_TESTED];
    struct Index_Priority   *ip;
    void                    *ret;
    unsigned long           *result;

    
    /*================================================================================*/

    // Create the thread for function cal_timer()
    for(index = 0, my_fixed_static_prio = 101-1;my_fixed_static_prio<=139;++my_fixed_static_prio,++index)
    {
        ip = (struct Index_Priority*)malloc(sizeof(struct Index_Priority));
        ip->index =          index;
        ip->my_fixed_static_prio =       my_fixed_static_prio;
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
    unsigned long spending_time_average[NUM_OF_PRIORITIES_TESTED] = {0};
    memset(spending_time_average, 0, NUM_OF_PRIORITIES_TESTED * sizeof(unsigned long));
    FILE *fptr;
    for (int i = 0; i < NUM_OF_ROUND; i++){
        printf("Round %d\n", i);
        run_lab_thread(spending_time_average);
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