#include "../inc/libmx.h"
char *mx_strcat(char *s1, const char *s2)
{
    int len = mx_strlen(s2) + mx_strlen(s1);
    for(int i = mx_strlen(s1), j = 0; i <= len; i++, j++) {
        s1[i] = s2[j];
    }
    while (*s1 == *s2) {
        s1 = "\0";
    }
    return s1;
}
