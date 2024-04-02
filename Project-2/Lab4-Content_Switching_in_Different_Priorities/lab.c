#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <stdint.h>

#include "./../src/header.h"
unsigned long spending_time_average[NUM_OF_PRIORITIES_TESTED] = {0};
uint64_t count_content_switching[NUM_OF_PRIORITIES_TESTED] = {0};

int main()
{
    memset(spending_time_average, 0, NUM_OF_PRIORITIES_TESTED * sizeof(unsigned long));
    FILE *fptr_spending_time_average, *fptr_count_content_switching;
    for (int i = 0; i < NUM_OF_ROUND; i++){
        printf("Round %d\n", i);

        run_lab_with_count_content_switching(spending_time_average, count_content_switching);
        for (int j = 0; j < NUM_OF_PRIORITIES_TESTED; j++){
            if (!j){
                fptr_spending_time_average          = fopen("spending_time_average.txt", "w");
                fptr_count_content_switching        = fopen("count_content_switching.txt", "w");
            }   
            else{
                fptr_spending_time_average          = fopen("spending_time_average.txt", "a");
                fptr_count_content_switching        = fopen("count_content_switching.txt", "a");
                fprintf(fptr_spending_time_average, "\n");
                fprintf(fptr_count_content_switching, "\n");
                
            }
            fprintf(fptr_spending_time_average, "%ld", spending_time_average[j]);
            fprintf(fptr_count_content_switching, "%ld", count_content_switching[j]);
            fclose(fptr_spending_time_average);
            fclose(fptr_count_content_switching);
        }
        printf("\n=======================================================\n");
    }
}