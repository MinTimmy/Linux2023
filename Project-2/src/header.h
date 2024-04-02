#include <stdint.h>
#include <time.h>
#include <sys/time.h>

// #define TOTAL_ITERATION_NUM  1000000
#define TOTAL_ITERATION_NUM  10000000
#define NUM_OF_PRIORITIES_TESTED  40
#define NUM_OF_ROUND 50


extern int my_set_process_priority(int my_fixed_priority, int my_fixed_static_prio, unsigned int my_fixed_rt_priority, uint64_t my_fixed_vruntime);
// static inline unsigned long cal_spending_time(struct timeval *start, struct timeval *end);
// static inline void lab_for_function();
extern void run_lab(unsigned long *spending_time_average);
unsigned long cal_spending_time(struct timeval *start, struct timeval *end);
void run_lab_with_count_content_switching(unsigned long *spending_time_average, uint64_t *count_content_switching);

struct Data
{
    int				        on_rq;
	int				        prio;
	int				        static_prio;
	int				        normal_prio;
	unsigned int			rt_priority;
    uint64_t                vruntime;
};
