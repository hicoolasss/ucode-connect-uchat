#include "../inc/libmx.h"
void mx_str_reverse(char *s)
{
    int l = mx_strlen(s);
    char *s1 = s;
    char *s2 = s;
    for (int i = 0; i < l - 1; i++)
        s2++;
    for (int i = 0; i < l / 2; i++) {
        mx_swap_char(s1, s2);
        s1++;
        s2--;
    }
}
