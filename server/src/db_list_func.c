#include "../inc/server.h"

// t_user *create_user_list(const char *db_username)
// {
//     t_user *clients = (t_user *)malloc(sizeof(t_user));
//     clients->username = mx_strdup((const char *)db_username);
//     mx_push_back(&client_list, clients);
//     return clients;
// }

t_list *get_clients(sqlite3 *db)
{
    int rc;
    sqlite3_stmt *stmt;
    t_list *users_list = NULL;
    const char *sql = "SELECT username FROM users;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *db_username = sqlite3_column_text(stmt, 0);
        t_user *user = (t_user *)malloc(sizeof(t_user));
        user->username = mx_strdup((const char *)db_username);
        if (user != NULL)
        {
            mx_push_back(&users_list, user);
        }
    }
    sqlite3_finalize(stmt);
    return users_list;
}

t_list *get_friends(sqlite3 *db, int user_id)
{
    char sql[100];
    sprintf(sql, "SELECT friend_id FROM friends WHERE user_id='%d'", user_id);
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: failed to execute statement: %s.\n", sqlite3_errmsg(db));
        return NULL;
    }
    // t_list *head = NULL;
    t_list *friend_list = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int friend_id = sqlite3_column_int(stmt, 0);
        char sql2[100];
        sprintf(sql2, "SELECT * FROM users WHERE id='%d'", friend_id);
        sqlite3_stmt *stmt2;
        int result2 = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
        if (result2 != SQLITE_OK)
        {
            fprintf(stderr, "Error: failed to execute statement: %s.\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return NULL;
        }
        if (sqlite3_step(stmt2) == SQLITE_ROW)
        {
            const unsigned char *db_username = sqlite3_column_text(stmt2, 1);
            t_user *friend_person = (t_user *)malloc(sizeof(t_user));
            friend_person->username = mx_strdup((const char *)db_username);
            if (friend_person != NULL)
            {
                mx_push_back(&friend_list, friend_person);
            }
        }
        sqlite3_finalize(stmt2);
    }
    sqlite3_finalize(stmt);
    return friend_list;
}
