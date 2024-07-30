#include "paging.h"
#include "../heap/kheap.h"
#include "../../status.h"

extern void paging_load_directory(uint32_t *directory);

static uint32_t* current_directory = 0;
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags) 
{
    uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) 
    {
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for (int b = 0; b < PAGING_TOTAL_ENTRIES_PER_TABLE; b++) 
        {
            entry[b] = (offset + (b * PAGING_PAGE_SIZE)) | flags;
        }
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t) entry | flags | PAGING_IS_WRITABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}

void paging_switch(uint32_t *directory) 
{
    paging_load_directory(directory);
    current_directory = directory;
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk) 
{
    return chunk->directory_entry;
}

bool paging_is_aligned(void* addr) 
{
    return ((uint32_t) addr % PAGING_PAGE_SIZE) == 0;
}

int paging_get_indexes(void* virtual_addr, uint32_t* directory_index_out, uint32_t* table_index_out) 
{
    int result = 0;
    if ( !paging_is_aligned(virtual_addr) ) 
    {
        result = -EINVARG;
        goto out;
    }

    *directory_index_out = (uint32_t) virtual_addr / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    *table_index_out     = (uint32_t) virtual_addr % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);

out:
    return result;
}

int paging_set(uint32_t *directory, void *vaddr, uint32_t val)
{
    if ( !paging_is_aligned(vaddr) )
    {
        return -EINVARG;
    }

    uint32_t directory_index = 0;
    uint32_t table_index     = 0;
    int result = paging_get_indexes(vaddr, &directory_index, &table_index);
    if ( result < 0 )
    {
        return result;
    }

    return 0;
}