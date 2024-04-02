#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>


#include "./../src/header.h"
unsigned long spending_time_average[NUM_OF_PRIORITIES_TESTED] = {0};


int main()
{
    memset(spending_time_average, 0, NUM_OF_PRIORITIES_TESTED * sizeof(unsigned long));
    FILE *fptr;
    for (int i = 0; i < NUM_OF_ROUND; i++){
        printf("Round %d\n", i);

        run_lab(spending_time_average);
        for (int j = 0; j < NUM_OF_PRIORITIES_TESTED; j++){
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
