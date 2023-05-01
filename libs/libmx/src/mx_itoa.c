#include "../inc/libmx.h"

#define digits 19

char *mx_itoa(int number) {
    static char buf[digits + 2];
    char *p = buf + digits + 1;
    if (number == 0) return mx_strcpy(buf, "0");
    if (number > 0) {
        while (number != 0) {
            *--p = '0' + (number % 10);
            number /= 10;
        }
        return p;
    } else { 
        while (number != 0) {
            *--p = '0' - (number % 10);
            number /= 10;
        }
        *--p = '-';
    }
    return p;
}
