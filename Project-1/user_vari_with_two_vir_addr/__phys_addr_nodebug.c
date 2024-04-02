#include <stdio.h>

#define __START_KERNEL_map 0xffffffff80000000
#define PAGE_OFFSET 0xffff9b81c0000000
// #define PAGE_OFFSET 0xffff922580000000
// #define phys_base 0x177600000
#define phys_base 0x000000000f000000

unsigned long __phys_addr_nodebug(unsigned long x)
{
	unsigned long y = x - __START_KERNEL_map;

    printf("y: 0x%016lx\n", y);
    printf("x > y: %d\n", x > y);
    printf("(x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET) 0x%016lx\n", (x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));
	/* use the carry flag to determine if x was < __START_KERNEL_map */
	x = y + ((x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));

	return x;
}

int main()
{
    // unsigned long virtual_addr = 0xffff9b826043d010;
    // unsigned long physical_addr = 0;
    // physical_addr = __phys_addr_nodebug(virtual_addr);

    // printf("physical_addr: 0x%016lx\n", physical_addr);
    // // unsigned long res = __START_KERNEL_map - PAGE_OFFSET;
    // printf("__START_KERNEL_map - PAGE_OFFSET: 0x%016lx\n", (unsigned long)__START_KERNEL_map - (unsigned long)PAGE_OFFSET);
    printf("0x%lx\n", (unsigned long)(0xffffffff80000000-1)  - (unsigned long)0xffffffff80000000);
    printf("0x%lx\n", (unsigned long)(0xffffffff80000000-1));
}