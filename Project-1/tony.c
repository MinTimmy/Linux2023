#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SYSCALL_v2p 452
#define SYSCALL_get_segetments_addr 453

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
const char k1 = 'a';
char var1 = 'a';
char var2;

struct segment
{
    unsigned long start;
    unsigned long end;
};

struct memory_set
{
    unsigned long mm_start_code;
    unsigned long mm_end_code;
    unsigned long mm_start_data;
    unsigned long mm_end_data;
    unsigned long mm_start_brk;
    unsigned long mm_brk;
    unsigned long mm_mmap_base;
    unsigned long mm_start_stack;

    struct segment vma_start_code;
    struct segment vma_end_code;
    struct segment vma_start_data;
    struct segment vma_end_data;
    struct segment vma_start_brk;
    struct segment vma_brk;
    struct segment vma_mmap_base;
    struct segment vma_start_stack;
};

unsigned long v2p(unsigned long vaddr)
{
    return syscall(SYSCALL_v2p, vaddr); // sys_v2p
}

void print_info(struct memory_set *mm_set, const char *prefix)
{
    printf("%s.mm.start_code: 0x%lx\n", prefix, mm_set->mm_start_code);
    printf("%s.mm.end_code: 0x%lx\n", prefix, mm_set->mm_end_code);
    printf("%s.mm.start_data: 0x%lx\n", prefix, mm_set->mm_start_data);
    printf("%s.mm.end_data: 0x%lx\n", prefix, mm_set->mm_end_data);
    printf("%s.mm.start_brk: 0x%lx\n", prefix, mm_set->mm_start_brk);
    printf("%s.mm.brk: 0x%lx\n", prefix, mm_set->mm_brk);
    printf("%s.mm.mmap_base: 0x%lx\n", prefix, mm_set->mm_mmap_base);
    printf("%s.mm.start_stack: 0x%lx\n", prefix, mm_set->mm_start_stack);

    printf("%s.vma.start_code.vm_start: 0x%lx\n", prefix, mm_set->vma_start_code.start);
    printf("%s.vma.start_code.vm_end: 0x%lx\n", prefix, mm_set->vma_start_code.end);
    printf("%s.vma.end_code.vm_start: 0x%lx\n", prefix, mm_set->vma_end_code.start);
    printf("%s.vma.end_code.vm_end: 0x%lx\n", prefix, mm_set->vma_end_code.end);
    printf("%s.vma.start_data.vm_start: 0x%lx\n", prefix, mm_set->vma_start_data.start);
    printf("%s.vma.start_data.vm_end: 0x%lx\n", prefix, mm_set->vma_start_data.end);
    printf("%s.vma.end_data.vm_start: 0x%lx\n", prefix, mm_set->vma_end_data.start);
    printf("%s.vma.end_data.vm_end: 0x%lx\n", prefix, mm_set->vma_end_data.end);
    printf("%s.vma.start_brk.vm_start: 0x%lx\n", prefix, mm_set->vma_start_brk.start);
    printf("%s.vma.start_brk.vm_end: 0x%lx\n", prefix, mm_set->vma_start_brk.end);
    printf("%s.vma.brk.vm_start: 0x%lx\n", prefix, mm_set->vma_brk.start);
    printf("%s.vma.brk.vm_end: 0x%lx\n", prefix, mm_set->vma_brk.end);
    printf("%s.vma.mmap_base.vm_start: 0x%lx\n", prefix, mm_set->vma_mmap_base.start);
    printf("%s.vma.mmap_base.vm_end: 0x%lx\n", prefix, mm_set->vma_mmap_base.end);
    printf("%s.vma.start_stack.vm_start: 0x%lx\n", prefix, mm_set->vma_start_stack.start);
    printf("%s.vma.start_stack.vm_end: 0x%lx\n", prefix, mm_set->vma_start_stack.end);
}

struct memory_set *thread_fn(void *name)
{
    pthread_mutex_lock(&mutex);

    pthread_attr_t attr;
    void *stack_addr;
    size_t stack_size;

    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);

    struct memory_set *mm_set = malloc(sizeof(struct memory_set));
    syscall(SYSCALL_get_segetments_addr, (unsigned long)stack_addr, mm_set); // sys_get_segments

    pthread_mutex_unlock(&mutex);
    return mm_set;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    struct memory_set *set_thread_1;
    struct memory_set *set_thread_2;

    pthread_mutex_lock(&mutex);

    pthread_create(&t1, NULL, thread_fn, NULL);
    pthread_create(&t2, NULL, thread_fn, NULL);

    pthread_mutex_unlock(&mutex);

    pthread_join(t1, &set_thread_1);
    pthread_join(t2, &set_thread_2);

    struct memory_set *set_main = malloc(sizeof(struct memory_set));
    syscall(SYSCALL_get_segetments_addr, NULL, set_main); // sys_get_segments
    // printf("\n========== Main ==========\n");
    print_info(set_main, "main");
    // printf("\n========== Thread 1 ==========\n");
    print_info(set_thread_1, "thread_1");
    // printf("\n========== Thread 2 ==========\n");
    print_info(set_thread_2, "thread_2");
    pthread_mutex_destroy(&mutex);
    return 0;
}