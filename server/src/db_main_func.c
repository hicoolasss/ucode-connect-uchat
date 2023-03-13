#include "../inc/head_db.h"

int db_log_to_serv(char *login, char *password, SSL *ssl)
{
    sql_create_db();
    char *log_id_check = sql_get("id", "login", login, 1);
    char *pass_id_check = sql_get("id", "password", password, 1);

    if ((log_id_check != NULL) && (pass_id_check != NULL))
    {
        int log_check = mx_atoi(log_id_check);
        int pass_check = mx_atoi(pass_id_check);

        if (log_check == pass_check)
        {
            mx_printstr("You are in chat(entered)!\n\n");
            return 0;
        }
    }
    else if ((log_id_check == NULL) && (pass_id_check == NULL)) {
        mx_printstr("user not found\n\n");
        return 1;
    }
    else if ((log_id_check != NULL) && (pass_id_check == NULL))
    {
        mx_printstr("incorrect password\n\n");
        return 2;
    }
    return 3;
}




int db_regestr_to_serv(char *login, char *password, SSL *ssl)
{
    sql_create_db();
    if (sql_set_user(login, password, ssl) == 1)
    {
        printf("this user already exist\n");
        return 1;
    }
    else
    {
        printf("You was regestered :-)\n\n");
        return 0;
    }


}

// t_list *get_login_list() {
//     sqlite3 *db;
//     sqlite3_stmt *stmt;
//     int result;
//     t_list *head = NULL;
//     t_list *current = NULL;
//     sql_open_db(&db);

//     if (sqlite3_prepare_v2(db, "SELECT login FROM User", -1, &stmt, NULL) != SQLITE_OK) {
//         fprintf(stderr, "Error sql in text %s\n", sqlite3_errmsg(db));
//         sqlite3_close(db);
//         return NULL;
//     }

//     while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
//         char *login = strdup((const char *) sqlite3_column_text(stmt, 0));
//         t_list *node = malloc(sizeof(t_list));
//         node->data = login;
//         node->next = NULL;

//         if (head == NULL) {
//             head = node;
//             current = head;
//         } else {
//             current->next = node;
//             current = current->next;
//         }
//     }

//     if (result != SQLITE_DONE) {
//         fprintf(stderr, "Ошибка выполнения SQL-запроса: %s\n", sqlite3_errmsg(db));
//         sqlite3_finalize(stmt);
//         sqlite3_close(db);
//         return NULL;
//     }

//     sqlite3_finalize(stmt);
//     sqlite3_close(db);
//     return head;
// }

// void print_login_list(t_list *head) {
//     t_list *current = head;

//     while (current != NULL) {
//         printf("%c\n", current->data);
//         current = current->next;
//     }
// }
