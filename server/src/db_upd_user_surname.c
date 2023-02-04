#include"../inc/server.h"

void update_user_surname(char *surname, int id) {
    char *query = NULL;
    char *sql_pattern = NULL;
    sql_pattern = "UPDATE users SET surname = '%s' WHERE id = %d;";
    asprintf(&query, sql_pattern, surname, id);
    sqlite3_exec_db(query, 2);
}
