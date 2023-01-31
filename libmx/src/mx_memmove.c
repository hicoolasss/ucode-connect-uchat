#include "../inc/libmx.h"
void *mx_memmove (void *dst, const void *src, size_t len) {
    char *d = dst;
    const char *s = src;
    if (d < s) {
        while (len--)
            *d++ = *s++;
    }
    else {
        char *lasts = (char *)s + (len-1);
        char *lastd = d + (len-1);
        while (len--)
            *lastd-- = *lasts--;
    }
    return dst;
}
