#include"../inc/server.h"

int get_user_id(char *login) {
    char *query = NULL;
    char *sql_pattern = "SELECT id FROM users WHERE login = ('%s');";
    asprintf(&query, sql_pattern, login);
    t_list *list = sqlite3_exec_db(query, 1);
    int u_id = mx_atoi(list->data);
    return u_id;
}