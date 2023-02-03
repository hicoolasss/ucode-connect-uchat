#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    int len = mx_strlen(sub);
    int count = 0;

    const char *temp = str;

    if (len) 
    while ((temp = mx_strstr(temp, sub))) {
        temp += len;
        count++;
    }
    return count;
}
