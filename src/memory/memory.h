#ifndef __MEMORY_H
#define __MEMORY_H

#include <stddef.h>

void * mem_set(void * ptr, int c, size_t size);
int    mem_cmp(void * s1, void * s2, int count);

#endif // __MEMORY_H
