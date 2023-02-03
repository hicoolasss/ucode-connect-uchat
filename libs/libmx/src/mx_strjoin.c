#include "../inc/libmx.h"
char *mx_strjoin(char const *s1, char const *s2) {
    if (s1 == NULL && s2 == NULL)
        return NULL;
    if (s1 == NULL) 
        return mx_strdup(s2);
    if (s2 == NULL)
        return mx_strdup(s1);
    
    int size = (mx_strlen(s1) + mx_strlen(s2) + 1);
    char *buf = mx_strnew(size);
    buf = mx_strcat(buf, s1);
    buf = mx_strcat(buf, s2);
    return buf;
}
