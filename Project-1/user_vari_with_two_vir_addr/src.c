#include <stdio.h>


#include "header.h"

void border()
{
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void show_page_table_value(struct Page_Table_Value *page_table_value)
{
    printf("page_frame_address       = 0x%016lx\n"          , page_table_value->page_frame_address                                      );
    printf("pgd_val   (pgd_index)    = 0x%016lx (%lu)\n"    , page_table_value->pgd_val            , page_table_value->pgd_index        );
    printf("p4d_val   (p4d_index)    = 0x%016lx (%lu)\n"    , page_table_value->p4d_val            , page_table_value->p4d_index        );
    printf("pud_val   (pud_index)    = 0x%016lx (%lu)\n"    , page_table_value->pud_val            , page_table_value->pud_index        );
    printf("pmd_val   (pmd_index)    = 0x%016lx (%lu)\n"    , page_table_value->pmd_val            , page_table_value->pmd_index        );
    printf("pte_val   (pte_index)    = 0x%016lx (%lu)\n"    , page_table_value->pte_val            , page_table_value->pte_index        );
    printf("page_addr (page_offset)  = 0x%016lx (%lu)\n"    , page_table_value->page_addr          , page_table_value->page_offset      );
    printf("virtual_address          = 0x%016lx\n"          , page_table_value->virtual_address                                         );
    printf("physical_address         = 0x%016lx\n"          , page_table_value->physical_address                                        );
    printf("value                    = %lu\n"               , page_table_value->value                                                   );
    border();
}




