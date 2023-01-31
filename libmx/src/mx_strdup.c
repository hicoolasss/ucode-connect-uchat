#include "../inc/libmx.h"
char *mx_strdup(const char *str) {
    int size = mx_strlen(str);
    char *src = mx_strnew(size);
    mx_strcpy (src,str);
    return src;
}
