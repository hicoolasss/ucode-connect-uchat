#include"../inc/server.h"

char *get_user_login(int id) {
    char *query = NULL;
    char *sql_pattern = "SELECT login FROM users WHERE id = (%d);";
    asprintf(&query, sql_pattern, id);//выделяет память под строку и выводит
    t_list *list = sqlite3_exec_db(query, 1);
    char *login = list->data;
    return login;
}
