#include "malloc_or_die.h"
#include <stdio.h>

void *malloc_or_die(size_t size)
{
    void *alloc_mem = malloc(size);

    if (alloc_mem == NULL && size) {
        printf("Error allocating memory.");
        abort();
    }

    return alloc_mem;
}
