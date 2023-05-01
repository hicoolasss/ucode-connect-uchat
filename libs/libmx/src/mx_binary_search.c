#include "../inc/libmx.h"
int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        *count += 1;
        int mid = low + (high - low) / 2;

        if (mx_strcmp(arr[mid], s) == 0)
            return mid;

        if (mx_strcmp(arr[mid], s) < 0)
            low = mid + 1;

        else
            high = mid - 1;
    }
    *count = 0;
    return -1;
}
