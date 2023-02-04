#include"../inc/server.h"

void db_delete_message(int m_id) {
    char *query = NULL;
    char *sql_pattern = NULL;
    sql_pattern = "DELETE FROM messages WHERE id = %d;";
    asprintf(&query, sql_pattern, m_id);
    sqlite3_exec_db(query, 2);
}

// void clear_message(char *mes, const int size) {
//     for (int i = 0; i < size; i++) {
//         if(mes[i] == '\0') {
//             break;
//         }
//         mes[i] = '\0';
//     }
// }