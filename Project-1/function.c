#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <string.h>

#include "./header.h"


void show_page_table_value(struct Page_Table_Value *page_table_value)
{
    printf("pgd_val   (pgd_index)   = 0x%016lx (%lu)\n",    page_table_value->pgd_val,      page_table_value->pgd_index   );
    printf("p4d_val   (p4d_index)   = 0x%016lx (%lu)\n",    page_table_value->p4d_val,      page_table_value->p4d_index   );
    printf("pud_val   (pud_index)   = 0x%016lx (%lu)\n",    page_table_value->pud_val,      page_table_value->pud_index   );
    printf("pmd_val   (pmd_index)   = 0x%016lx (%lu)\n",    page_table_value->pmd_val,      page_table_value->pmd_index   );
    printf("pte_val   (pte_index)   = 0x%016lx (%lu)\n",    page_table_value->pte_val,      page_table_value->pte_index   );
    printf("page_addr (page_offset) = 0x%016lx (%lu)\n",    page_table_value->page_addr,    page_table_value->page_offset );
    printf("virtual_address         = 0x%016lx\n",          page_table_value->virtual_address   );
    printf("physical_address        = 0x%016lx\n",          page_table_value->physical_address  );
    border();
}

void __show_segment(char *name, struct Segment *s)
{
    // printf("%s,", name);
    printf("%s\n", name);
    // printf("0x%016lx,0x%016lx,0x%016lx,0x%016lx,", 
    printf("(vma_start) start: 0x%016lx (0x%016lx), end = 0x%016lx (0x%016lx)\n", 
        s->vma_start.start, virtual_address_to_physical_address(s->vma_start.start, NULL),  
        s->vma_start.end,   virtual_address_to_physical_address(s->vma_start.end,   NULL) 
    );
    // printf("0x%016lx,0x%016lx,0x%016lx,0x%016lx\n", 
    printf("(vma_end)   start: 0x%016lx (0x%016lx), end = 0x%016lx (0x%016lx)\n", 
        s->vma_end.start, virtual_address_to_physical_address(s->vma_end.start, NULL),  
        s->vma_end.end,   virtual_address_to_physical_address(s->vma_end.end ,  NULL) 
    );
}
void show_segment(struct Process_Struct *process_struct)
{
    // struct Process_Struct *process_struct  = malloc(sizeof(struct Process_Struct));
    syscall(338, process_struct);

    char *name = malloc(10);

    strcpy(name, "text");
    __show_segment(name, &process_struct->text);
    strcpy(name, "data");
    __show_segment(name, &process_struct->data);
    strcpy(name, "bss");
    __show_segment(name, &process_struct->bss);
    strcpy(name, "heap");
    __show_segment(name, &process_struct->heap);
    strcpy(name, "mm");
    __show_segment(name, &process_struct->mm);
    strcpy(name, "stack");
    __show_segment(name, &process_struct->stack);
    
    // free(process_struct);
    free(name);
}

void border()
{
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}


void show_process_struct(struct Process_Struct *process_struct)
{
    // struct Process_Struct *process_struct  = malloc(sizeof(struct Process_Struct));
    syscall(338, process_struct);

    // struct Data *data = malloc(sizeof(struct Data));

    printf("current->mm->start_brk      = 0x%016lx (0x%016lx)\n", process_struct->my_mm.start_brk,        virtual_address_to_physical_address(process_struct->my_mm.start_brk,   NULL) );
    printf("current->mm->brk            = 0x%016lx (0x%016lx)\n", process_struct->my_mm.brk,              virtual_address_to_physical_address(process_struct->my_mm.brk,         NULL) );
    printf("current->mm->start_stack    = 0x%016lx (0x%016lx)\n", process_struct->my_mm.start_stack,      virtual_address_to_physical_address(process_struct->my_mm.start_stack, NULL) );
    printf("current->mm->mmap_base      = 0x%016lx (0x%016lx)\n", process_struct->my_mm.mmap_base,        virtual_address_to_physical_address(process_struct->my_mm.mmap_base,   NULL) );
    printf("current->mm->task_size      = %lu bytes          \n", process_struct->my_mm.task_size          );
    printf("current->mm->start_code     = 0x%016lx (0x%016lx)\n", process_struct->my_mm.start_code,       virtual_address_to_physical_address(process_struct->my_mm.start_code,  NULL) );
    printf("current->mm->end_code       = 0x%016lx (0x%016lx)\n", process_struct->my_mm.end_code,         virtual_address_to_physical_address(process_struct->my_mm.end_code,    NULL) );
    printf("current->mm->start_data     = 0x%016lx (0x%016lx)\n", process_struct->my_mm.start_data,       virtual_address_to_physical_address(process_struct->my_mm.start_data,  NULL) );
    printf("current->mm->end_data       = 0x%016lx (0x%016lx)\n", process_struct->my_mm.end_data,         virtual_address_to_physical_address(process_struct->my_mm.end_data,    NULL) );

    border();

    printf("current->stack              = 0x%016lx (0x%016lx)\n", process_struct->my_task_struct.stack,   virtual_address_to_physical_address(process_struct->my_task_struct.stack,NULL) );
    printf("current->mm                 = 0x%016lx (0x%016lx)\n", process_struct->my_task_struct.mm,      virtual_address_to_physical_address(process_struct->my_task_struct.mm,   NULL) );
    printf("current->stack_canary       = 0x%016lx           \n", process_struct->my_task_struct.stack_canary     );
}

unsigned long virtual_address_to_physical_address(unsigned long virtual_address, struct Data *data)
{
    if (data != NULL){
            data->user.virtual_address  = virtual_address;
        data->user.value            = *(unsigned long*)virtual_address;

        syscall(340, (void *)data);
        return data->user.physical_address;
    }
    else{
        return syscall(335, virtual_address);
    }

}

