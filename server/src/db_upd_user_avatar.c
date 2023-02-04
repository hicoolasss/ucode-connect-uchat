#include"../inc/server.h"

void update_user_avatar(char *path, int id) {
    char *query = NULL;
    char *sql_pattern = NULL;
    sql_pattern = "UPDATE users SET avatar = '%s' WHERE id = %d;";
    asprintf(&query, sql_pattern, path, id);
    sqlite3_exec_db(query, 2);
}
