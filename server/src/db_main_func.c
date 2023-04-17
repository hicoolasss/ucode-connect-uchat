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

int sql_create_new_group(sqlite3 *db, const char *groupname, const char *avatarname, const char *avatarblob, int avatarsize)
{
    int rc;
    sqlite3_stmt *stmt;

    if (mx_strcmp(avatarname, "null") == 0 && mx_strcmp(avatarblob, "null") == 0 && avatarsize == 0)
    {
        char *sql = "INSERT INTO newgroups (groupname) VALUES (?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        sqlite3_bind_text(stmt, 1, groupname, -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        char *sql = "INSERT INTO newgroups (groupname, avatarname, avatardata VALUES (?, ?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        sqlite3_bind_text(stmt, 1, groupname, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 1, avatarname, mx_strlen(avatarname), SQLITE_TRANSIENT);
        sqlite3_bind_blob(stmt, 2, avatarblob, avatarsize, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
    }
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int add_user_to_chat(sqlite3 *db, int group_id, int user_id)
{
    int rc;
    sqlite3_stmt *stmt;
    char insert_participant_sql[] = "INSERT INTO group_users (group_id, user_id) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_participant_sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_int(stmt, 1, group_id);
    sqlite3_bind_int(stmt, 2, user_id);

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

t_chat *sql_record_message(sqlite3 *db, char *username, char *friendname, const char *message_text) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO dialogs (user_id, friend_id, message) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    int user_id = get_user_id(db, username);
    int friend_id = get_user_id(db, friendname);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);
    sqlite3_bind_text(stmt, 3, message_text, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Ошибка выполнения SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }
    int message_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);

    const char *sql2 = "SELECT timestamp FROM dialogs WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Ошибка подготовки SQL: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, message_id);
    t_chat *message_data = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        message_data = (t_chat *)malloc(sizeof(t_chat));
        message_data->id = message_id;
        message_data->message = mx_strdup(message_text);
        message_data->timestamp = mx_strdup((const char *)sqlite3_column_text(stmt, 0));
        message_data->sender = mx_strdup(username);
    } else {
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
    if(result == SQLITE_ROW) {
        lastmessage = (char *)sqlite3_column_text(stmt, 0);
        return lastmessage;
    }
    else if (result == SQLITE_DONE)
    {
        // printf("No message found in the dialog.\n");
        sqlite3_finalize(stmt);
        return NULL;
    }
    else
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }
}
