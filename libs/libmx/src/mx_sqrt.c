#include "../inc/libmx.h"
int mx_sqrt(int x) {
    int div = 1, result = 0;
    while (x > 0) {
        x -= div, div += 2;
        result += x < 0 ? 0 : 1;
    }
    if (x < 0) {
        return 0;
    } else {
        return result;
    }
}
