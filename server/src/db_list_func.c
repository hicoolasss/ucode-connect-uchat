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

t_list *get_friends(sqlite3 *db, const char *username)
{
    int user_id = get_user_id(db, username);
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
            sqlite3_finalize(stmt2);
            sqlite3_finalize(stmt);
            return NULL;
        }
        if (sqlite3_step(stmt2) == SQLITE_ROW)
        {
            const char *friendname = (const char *)sqlite3_column_text(stmt2, 1);
            char *lastmessage = get_last_message_from_dialog(db, username, friendname);
            t_user *friend_person = (t_user *)malloc(sizeof(t_user));
            if (lastmessage != NULL)
            {
                friend_person->lastmessage = mx_strdup((const char *)lastmessage);
            }
            else
                friend_person->lastmessage = NULL;
            friend_person->username = mx_strdup((const char *)friendname);
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

t_list *get_message_history(sqlite3 *db, int user_id, int friend_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT user_id, message, timestamp, id FROM dialogs WHERE (user_id = ? AND friend_id = ?) OR (user_id = ? AND friend_id = ?) ORDER BY timestamp;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    t_list *chat_recording = NULL;
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);
    sqlite3_bind_int(stmt, 3, friend_id);
    sqlite3_bind_int(stmt, 4, user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int sender_id = sqlite3_column_int(stmt, 0);
        char *sender = get_username_by_user_id(db, sender_id);
        const unsigned char *text = sqlite3_column_text(stmt, 1);
        const unsigned char *timestamp = sqlite3_column_text(stmt, 2);
        int message_id = sqlite3_column_int(stmt, 3);

        t_chat *chat_record = (t_chat *)malloc(sizeof(t_chat));
        chat_record->id = message_id;
        chat_record->sender = mx_strdup(sender);
        chat_record->message = mx_strdup((const char *)text);
        chat_record->timestamp = mx_strdup((const char *)timestamp);

        if (chat_record)
        {
            mx_push_back(&chat_recording, chat_record);
        }
        // printf("[%s] %d: %s\n", timestamp, sender_id, text);
    }

    sqlite3_finalize(stmt);
    return chat_recording;
}
