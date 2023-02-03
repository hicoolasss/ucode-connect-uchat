#include "../inc/libmx.h"

char *mx_del_extra_spaces(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    int j = 0;
    char *newstr = mx_strnew(mx_strlen(str));
    for(int i = 0; i < mx_strlen(str); i++) {
        if(!(mx_isspace(str[i]))) {
            newstr[j] = str[i];
            j++;
        }
        if (!(mx_isspace(str[i])) && mx_isspace(str[i + 1])) {
            newstr[j] = ' ';
            j++;
        }
    }
    char *temp = mx_strtrim(newstr);
    mx_strdel(&newstr);
    return temp;
}
