#include"../inc/server.h"

char  *get_user_avatar(int id) {
    char *query = NULL;
    char *sql_pattern = "SELECT avatar FROM users WHERE id = (%d);";
    asprintf(&query, sql_pattern, id);
    t_list *list = sqlite3_exec_db(query, 1);
    char *login = list->data;
    return login;
}
