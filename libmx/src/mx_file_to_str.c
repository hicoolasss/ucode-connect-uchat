#include "../inc/libmx.h"

char *mx_file_to_str(const char *filename) {
    int size = 0;
    int i = 0;
    char c;
    int file = open(filename, O_RDONLY);
    if (file < 0) return NULL;
    while (read(file, &c, 1))
        size++;
    close(file);
    char *string = mx_strnew(size);
    file = open(filename, O_RDONLY);
    while (read(file, &c, 1)) {
        string[i] = c;
        i++;
    }
    close(file);
    return string;
}
