#include"../inc/server.h"

void update_chat_name(char *name, int id) {
    char *query = NULL;
    char *sql_pattern = NULL;
    sql_pattern = "UPDATE chats SET name = '%s' WHERE id = %d;";
    asprintf(&query, sql_pattern, name, id);
    sqlite3_exec_db(query, 2);
}
