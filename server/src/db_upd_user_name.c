#include"../inc/server.h"

void update_user_name(char *name, int id) {
    char *query = NULL;
    char *sql_pattern = NULL;
    sql_pattern = "UPDATE users SET name = '%s' WHERE id = %d;";
    asprintf(&query, sql_pattern, name, id);//https://ru.manpages.org/asprintf/3
    sqlite3_exec_db(query, 2);
}
