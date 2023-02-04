#include"../inc/server.h"

int get_chat_id(char *name) {
    char *query = NULL;
    char *sql_pattern = "SELECT id FROM chats WHERE name = ('%s');";
    asprintf(&query, sql_pattern, name);
    t_list *list = sqlite3_exec_db(query, 1);
    int c_id = mx_atoi(list->data);
    return c_id;
}
