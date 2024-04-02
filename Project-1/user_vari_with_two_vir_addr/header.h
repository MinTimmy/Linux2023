#include <pthread.h>


struct Page_Table_Value
{
    unsigned long                       pgd_val;
    unsigned long                       pgd_index;
    unsigned long                       p4d_val;
    unsigned long                       p4d_index;
    unsigned long                       pud_val;
    unsigned long                       pud_index;
    unsigned long                       pmd_val;
    unsigned long                       pmd_index;
    unsigned long                       pte_val;
    unsigned long                       pte_index;
    unsigned long                       page_addr;
    unsigned long                       page_offset;
    unsigned long                       virtual_address;
    unsigned long                       physical_address;
    unsigned long                       value;
    unsigned long                       page_frame_address;
};

struct Data
{
    struct Page_Table_Value kernel; 
    struct Page_Table_Value user;
};

struct data_
{
    int                     id ;
    char                    name[16] ;
    struct Process_Struct   *process_struct;
    struct Teacher_Target   *teacher_target;
} ;
typedef struct data_ sdata ;

void border();
void show_page_table_value(struct Page_Table_Value *page_table_value);
