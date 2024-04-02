#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>

#include "./../src/header.h"


int main(int argc, char *argv[])
{
    struct timeval   start, end;
    unsigned int my_fixed_rt_priority = (unsigned int)atoi(argv[2]);
    my_set_process_priority(0, 0, my_fixed_rt_priority, 0);
    sleep(5);
    gettimeofday(&start, NULL);           //begin
    for(int i=1;i<=TOTAL_ITERATION_NUM;i++){
         // printf("\00");
        rand();
        // sleep(1);
        // count++;
    }
    gettimeofday(&end, NULL);             //end     
    my_set_process_priority(0, 0, 0, 0);
    uint64_t spending_time = cal_spending_time(&start, &end);
    // pid_t pid = getpid();
    // printf("(%s) Priority: %d,Spending Time: %ld ms\n", argv[1], my_fixed_rt_priority, spending_time);
    printf("%ld", spending_time);
}