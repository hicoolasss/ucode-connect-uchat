#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    int size = sizeof(unsigned long);
    char *hexaDeciNum = mx_strnew(size);
    int i = 0;
    int len = 0;
    while (nbr != 0) {
        int temp = 0;
        temp = nbr % 16;
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
        nbr = nbr / 16;
        len++;
    }
     if (len < 1) {
        return NULL;
    }
    char *result = mx_strnew(len);
    for(int i = 0; i < len; i++){
        result[i] = hexaDeciNum[len - i - 1];
    }
    return result;
}
