#include "kheap.h"
#include "heap.h"
#include "../memory.h"
#include "../../config.h"
#include "../../kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init() 
{
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY *) HEAP_TABLE_ADDRESS;
    
    int total_table_entries = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;
    kernel_heap_table.total = total_table_entries;

    void * end = (void *) (HEAP_ADDRESS + HEAP_SIZE_BYTES);
    int result = heap_create(&kernel_heap, (void *) HEAP_ADDRESS, end, &kernel_heap_table);
    if ( result < 0 ) 
    {
        print_str("failed to create heap!!\n");
    }
}

void * kmalloc(size_t size) 
{
    return heap_malloc(&kernel_heap, size);
}

// kernel zero alloc
void * kzalloc(size_t size) 
{
    void* ptr = kmalloc(size);
    if (!ptr) 
    {
        return 0;
    }

    mem_set(ptr, 0x00, size);
    return ptr;
}

void kfree(void * ptr) 
{
    heap_free(&kernel_heap, ptr);
}
