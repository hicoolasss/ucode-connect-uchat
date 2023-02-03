#include "../inc/libmx.h"
char *mx_strstr(const char*haystack, const char*needle)
{
    //const char* ptr = haystack;
    while (*haystack) {
        if (mx_strncmp(haystack, needle, mx_strlen(needle)) == 0)
            return (char*)haystack;
        haystack++;
    }
    return NULL;
}
