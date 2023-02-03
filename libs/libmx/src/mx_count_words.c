#include "../inc/libmx.h"
int mx_count_words(const char *str, char delimiter) {
    int state = 1;
    int count = 0;
    while (*str) {
        if (*str == delimiter){
            state = 1;
        } else if (state == 1) {
            state = 0;
            ++count;
        }
        ++str;
    }
    return count;
}
