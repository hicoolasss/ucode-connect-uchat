#include "../inc/libmx.h"

bool mx_isalpha(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0)
        return true;
    else
        return false;
}
