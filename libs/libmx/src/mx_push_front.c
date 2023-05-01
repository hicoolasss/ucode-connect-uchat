#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *temp = mx_create_node(data);
    if (*list == NULL) {
        *list = temp;
    } else {
        temp->next = *list;
        *list = temp;
    }
}
