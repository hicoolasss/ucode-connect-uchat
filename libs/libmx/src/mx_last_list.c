#include "../inc/libmx.h"

t_list *mx_list_last(t_list *list) {
    if (!list) {
        return NULL;
    }

    while (list->next) {
        list = list->next;
    }

    return list;
}
