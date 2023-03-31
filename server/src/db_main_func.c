#include "../inc/server.h"

int authenticate_user(sqlite3 *db, const char *username, const char *password)
{
    int rc;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM users WHERE username=?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        const unsigned char *db_password = sqlite3_column_text(stmt, 2);

        if (strcmp((char *)db_password, password) == 0)
        {
            printf("Authentication successful\n");
            sqlite3_finalize(stmt);
            return 0;
        }
        else
        {
            printf("Incorrect password\n");
            sqlite3_finalize(stmt);
            return 2;
        }
    }
    else
    {
        printf("User not found\n");
        sqlite3_finalize(stmt);
        return 1;
    }
}

int register_user(sqlite3 *db, const char *username, const char *password)
{

    char *zErrMsg = 0;
    int rc;
    const char *sql_template = "INSERT INTO users (username, password) VALUES ('%s', '%s');";
    char *sql = sqlite3_mprintf(sql_template, username, password);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_free(sql);
        return 1;
    }
    else
    {
        printf("User registered successfully\n");
    }

    sqlite3_free(sql);
    return 0;
}

t_list *get_clients(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT username FROM users;";
    t_list *head = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *db_username = sqlite3_column_text(stmt, 0);
        t_list *clients = (t_list*)malloc(sizeof(t_list));
        clients->data = (char *)malloc(strlen((char *)db_username) + 1);
        // mx_strcpy(clients->data, (const char *)db_username);
        clients->data = mx_strdup((const char *)db_username);
        clients->next = NULL;

        // Добавление нового элемента в список
        if (head == NULL) {
            head = clients;
        } else {
            t_list *temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = clients;
        }
    }
    sqlite3_finalize(stmt);
    return head;
}

int add_friend(sqlite3 *db, const char *username, const char *friend_username)
{
    if (mx_strcmp(username, friend_username) == 0)
    {
        printf("Error: cannot add yourself as a friend.\n");
        return 0;
    }

    int user_id = get_user_id(db, username);
    int friend_id = get_user_id(db, friend_username);

    if (is_friend(db, user_id, friend_id))
    {
        printf("Error: %s is already your friend.\n", friend_username);
        return 0;
    }

    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO friends (user_id, friend_id) VALUES (?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    printf("%s added to friends.\n", friend_username);
    return 1;
}
