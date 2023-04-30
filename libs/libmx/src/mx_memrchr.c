#include "../inc/libmx.h"
void *mx_memrchr(const void *s, int c, size_t n) {
    unsigned char *cp;
    if (n != 0) {
	    cp = (unsigned char *)s + n;
	    while(n--) {
	        if (*cp == (unsigned char)c)
		        return cp;
            cp--;
	    }
    }
    return NULL;
}
