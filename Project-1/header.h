struct Page_Table_Value
{
    unsigned long               pgd_val;
    unsigned long               pgd_index;
    unsigned long               p4d_val;
    unsigned long               p4d_index;
    unsigned long               pud_val;
    unsigned long               pud_index;
    unsigned long               pmd_val;
    unsigned long               pmd_index;
    unsigned long               pte_val;
    unsigned long               pte_index;
    unsigned long               page_addr;
    unsigned long               page_offset;
    unsigned long               virtual_address;
    unsigned long               physical_address;
    unsigned long               value;
    unsigned long               page_frame_address;
};
struct Data
{
    struct Page_Table_Value kernel; 
    struct Page_Table_Value user;
};

// struct Teacher_Target
// {
//     struct Page_Table_Value     *global_variable_a;
//     struct Page_Table_Value     *local_variable_b;
//     struct Page_Table_Value     *thread_local_variable_tx;
//     struct Page_Table_Value     *function_hello;
//     struct Page_Table_Value     *function_func1;
//     struct Page_Table_Value     *function_func2;
//     struct Page_Table_Value     *function_main;
//     struct Page_Table_Value     *library_function_printf;
//     struct Page_Table_Value     *local_heap;
// };

struct Start_End
{
    unsigned long               start;
    unsigned long               end;
};
struct Segment
{
    unsigned long                 sample_value;
    struct Start_End              vma_start;  
    struct Start_End              vma_end;
};

struct Process_Struct
{
    struct Segment               text;
    struct Segment               data;
    struct Segment               bss;
    struct Segment               heap;
    struct Segment               mm;
    struct Segment               stack;
    unsigned long                stack_addr;

    struct My_mm
    {
        unsigned long               start_brk;
        unsigned long               brk;
        unsigned long               start_stack;
        unsigned long               mmap_base;
        unsigned long               task_size;

        // Code Segment
        unsigned long               start_code;
        unsigned long               end_code;

        // Data Segment
        unsigned long               start_data;
        unsigned long               end_data;
    } my_mm;

    struct My_Task_Struct
    {
        unsigned long               stack;
        unsigned long               stack_canary;
        // struct mm_struct            *mm;
        unsigned long               mm;
    }my_task_struct;
};

struct data_
{
    int                     id ;
    char                    name[16] ;
    // struct Process_Struct   *process_struct;
    // struct Teacher_Target   *teacher_target;
} ;
typedef struct data_ sdata ;

void show_page_table_value(struct Page_Table_Value *page_table_value);
void show_segment(struct Process_Struct *process_struct);
void show_process_struct(struct Process_Struct *process_struct);
void border();
unsigned long virtual_address_to_physical_address(unsigned long virtual_address, struct Data *data);
void __show_segment(char *name, struct Segment *s);
void show(int pid, struct Process_Struct *process_struct);
void __show_kernel_user_address(char *name, unsigned long virtual_address);
void show_kernel_user_address(struct Process_Struct *process_struct);


