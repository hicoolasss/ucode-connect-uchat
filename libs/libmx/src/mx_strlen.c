#include "../inc/libmx.h"
int mx_strlen(const char *s)
{
    int temp = 0;
    while (*s != '\0') {
        s++;
        temp++;
    }
    return temp;
}

