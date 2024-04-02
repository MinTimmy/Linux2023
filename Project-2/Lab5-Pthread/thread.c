#include "header.h"

int main()
{
    lab_real_time(SCHED_FIFO);
    lab_real_time(SCHED_RR);
    lab_normal();
    
    return 0;
}