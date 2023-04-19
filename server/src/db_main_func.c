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
    char *sql = "INSERT INTO friends (user_id, friend_id) VALUES (?, ?), (?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);
    sqlite3_bind_int(stmt, 3, friend_id);
    sqlite3_bind_int(stmt, 4, user_id);

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

int create_chat_record(sqlite3 *db, int chat_id, int sender_id, int recipient_id, const char *message)
{
    int rc;
    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO chats (chat_id, sender_id, recipient_id, message) VALUES (?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, chat_id);
    sqlite3_bind_int(stmt, 2, sender_id);
    sqlite3_bind_int(stmt, 3, recipient_id);
    sqlite3_bind_text(stmt, 4, message, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int create_group_chat(sqlite3 *db, const char *group_name, t_list *users)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO groups (group_name) VALUES (?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, group_name, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    int group_id = sqlite3_last_insert_rowid(db);

    for (t_list *iter = users; iter != NULL; iter = iter->next)
    {
        int user_id = get_user_id(db, ((t_user *)iter->data)->username);

        const char *sql_add_member = "INSERT INTO group_members (group_id, user_id) VALUES (?, ?);";

        if (sqlite3_prepare_v2(db, sql_add_member, -1, &stmt, NULL) != SQLITE_OK)
        {
            printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
            return -1;
        }

        sqlite3_bind_int(stmt, 1, group_id);
        sqlite3_bind_int(stmt, 2, user_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return -1;
        }

        sqlite3_finalize(stmt);
    }

    return group_id;
}

int add_group_message(sqlite3 *db, const char *group_name, const char *sender_name, const char *message_text)
{
    int group_id = get_group_id(db, group_name);
    int sender_id = get_user_id(db, sender_name);

    if (group_id == -1 || sender_id == -1)
    {
        printf("Ошибка: Невозможно найти группу или отправителя.\n");
        return -1;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO group_messages (group_id, sender_id, message) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, sender_id);
    sqlite3_bind_text(stmt, 3, message_text, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

t_chat *sql_record_message(sqlite3 *db, char *username, char *friendname, const char *message_text)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO dialogs (user_id, friend_id, message) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    int user_id = get_user_id(db, username);
    int friend_id = get_user_id(db, friendname);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);
    sqlite3_bind_text(stmt, 3, message_text, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }
    int message_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);

    const char *sql2 = "SELECT timestamp FROM dialogs WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, message_id);
    t_chat *message_data = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        message_data = (t_chat *)malloc(sizeof(t_chat));
        message_data->id = message_id;
        message_data->message = mx_strdup(message_text);
        message_data->timestamp = mx_strdup((const char *)sqlite3_column_text(stmt, 0));
        message_data->sender = mx_strdup(username);
    }
    else
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return message_data;
}

char *get_last_message_from_dialog(sqlite3 *db, const char *username, const char *friendname)
{
    sqlite3_stmt *stmt;
    char *lastmessage = NULL;
    char *sql = "SELECT message FROM dialogs WHERE (user_id = ? AND friend_id = ?) OR (user_id = ? AND friend_id = ?) ORDER BY timestamp DESC LIMIT 1;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    int user_id = get_user_id(db, username);
    int friend_id = get_user_id(db, friendname);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);
    sqlite3_bind_int(stmt, 3, friend_id);
    sqlite3_bind_int(stmt, 4, user_id);

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        const unsigned char *temp = sqlite3_column_text(stmt, 0);
        lastmessage = mx_strdup((const char *)temp);
    }
    else if (result == SQLITE_DONE)
    {
        lastmessage = NULL;
    }
    else
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        lastmessage = NULL;
    }

    sqlite3_finalize(stmt);
    return lastmessage;
}

int sql_delete_message_from_dialog(sqlite3 *db, int message_id, const char *username, const char *message)
{
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM dialogs WHERE id = ? AND user_id = ? AND message = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    int user_id = get_user_id(db, username);

    sqlite3_bind_int(stmt, 1, message_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_text(stmt, 3, message, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int sql_update_message_in_dialog(sqlite3 *db, int message_id, const char *old_message, const char *new_message, const char *username)
{
    sqlite3_stmt *stmt;
    char *sql = "UPDATE dialogs SET message = ? WHERE id = ? AND message = ? AND user_id = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    int user_id = get_user_id(db, username);

    sqlite3_bind_text(stmt, 1, new_message, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, message_id);
    sqlite3_bind_text(stmt, 3, old_message, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, user_id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}
