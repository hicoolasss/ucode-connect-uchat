#include "../inc/libmx.h"
t_list *mx_create_node(void *data) {
    t_list *list = (t_list*)malloc(8 * 2);
    list->data = data;
    list->next = NULL;
    return list;
}
