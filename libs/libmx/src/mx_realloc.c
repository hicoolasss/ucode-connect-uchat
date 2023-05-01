#include "../inc/libmx.h"
void *mx_realloc(void *ptr, size_t size)
{
    if (size == 0) {
        size = 1;
        free(ptr);
        ptr = NULL;
    }
    if (ptr == NULL)
        return malloc(size);
    return ptr;
}
