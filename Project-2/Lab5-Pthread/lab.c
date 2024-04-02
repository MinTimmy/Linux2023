#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>


#include "./../src/header.h"

// enum schedule_policies_enum
// {
//     SCHED_OTHER,
//     SCHED_BATCH,
//     SCHED_IDLE,
//     SCHED_FIFO,
//     SCHED_RR
// };

char *schedule_policies_array[] = {
    "SCHED_OTHER",
    "SCHED_BATCH",
    "SCHED_IDLE",
    "SCHED_FIFO",
    "SCHED_R"
};

void get_rlim()
{    
    struct rlimit *rlim         = (struct rlimit*)malloc(sizeof(struct rlimit));
    getrlimit(RLIMIT_NICE, rlim);
    printf("rlim_cur: %lu\n",                   rlim->rlim_cur);  /* Soft limit */
    printf("rlim_max: %lu\n",                   rlim->rlim_max);  /* Hard limit (ceiling for rlim_cur) */
}
int main()
{
    // int which                   = PRIO_PROCESS;
    // id_t pid                    = getpid();
    int ret                     = -1;
    // struct sched_param *param   = (struct sched_param*)malloc(sizeof(struct sched_param));
    // int scheduling_policy       = -1;
    // int nice_value              = -1;
    struct rlimit *rl               = (struct rlimit*)malloc(sizeof(struct rlimit));


    // param->sched_priority = 3;
    // ret = sched_setscheduler(pid, 0, param);
    // printf("ret: %d\n", ret);
    // scheduling_policy           = sched_getscheduler(pid);

    // setpriority(which, pid, 3); // change the value of nice
    // nice_value                  = getpriority(which, pid);
    
    get_rlim();
    // printf("scheduling policy: %d\n",           scheduling_policy);
    // printf("which: %d\n",                       which);
    // printf("pid: %d\n",                         pid);
    // printf("The nice value: %d\n",              nice_value);

    // param->sched_priority = 10;
    // sched_setparam(pid, param);
    
    // if (!ret) printf("param->sched_priority: %d\n", param->sched_priority);



    // my_set_process_priority(0, 0, 0, 0);
    rl->rlim_cur = rl->rlim_max = -19;
    ret = setrlimit(RLIMIT_NICE, rl);
    printf("ret: %d\n", ret);
    get_rlim();
    printf("sched_get_priority_max SCHED_OTHER %d\n",       sched_get_priority_max(SCHED_OTHER));
    printf("sched_get_priority_min SCHED_OTHER %d\n",       sched_get_priority_min(SCHED_OTHER));
    printf("sched_get_priority_max SCHED_RR %d\n",       sched_get_priority_max(SCHED_RR));
    printf("sched_get_priority_min SCHED_RR %d\n",       sched_get_priority_min(SCHED_RR));

    return 0;
}

