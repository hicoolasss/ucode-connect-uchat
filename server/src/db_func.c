#include "../inc/server.h"

sqlite3 *db_open()
{
    sqlite3 *db;

    if (sqlite3_open(DB_NAME, &db) != SQLITE_OK)
    {
        mx_printerr("Cannot open database");
        return NULL;
    }
    return db;
}

void sql_create_db()
{
    sqlite3 *db = db_open();
    char *err_msg = 0;
    int rc;

    const char *sql_users = "CREATE TABLE IF NOT EXISTS users ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "username TEXT NOT NULL UNIQUE,"
                            "password TEXT NOT NULL,"
                            "avatarname TEXT,"
                            "avatardata BLOB);";

    rc = sqlite3_exec(db, sql_users, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    const char *sql_friends = "CREATE TABLE IF NOT EXISTS friends ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "user_id INTEGER NOT NULL,"
                              "friend_id INTEGER NOT NULL,"
                              "FOREIGN KEY(user_id) REFERENCES users(id),"
                              "FOREIGN KEY(friend_id) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql_friends, NULL, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    char *sql_create_new_chat = "CREATE TABLE IF NOT EXISTS newgroups ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "groupname TEXT NOT NULL,"
                                "avatarname TEXT,"
                                "avatardata BLOB);";

    rc = sqlite3_exec(db, sql_create_new_chat, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    char *sql_groups = "CREATE TABLE IF NOT EXISTS groups ("
                       "message_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "group_id INTEGER NOT NULL,"
                       "sendler_id INTEGER NOT NULL,"
                       "message TEXT NOT NULL,"
                       "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
                       "FOREIGN KEY(group_id) REFERENCES newgroups(id),"
                       "FOREIGN KEY(sendler_id) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql_groups, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    char *sql_group_info = "CREATE TABLE IF NOT EXISTS group_users ("
                           "group_id INTEGER NOT NULL,"
                           "user_id INTEGER NOT NULL,"
                           "PRIMARY KEY(group_id, user_id),"
                           "FOREIGN KEY(group_id) REFERENCES newgroups(id),"
                           "FOREIGN KEY(user_id) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql_group_info, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    char sql_create_table_dialogs[] = "CREATE TABLE IF NOT EXISTS dialogs ("
                                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "user_id INTEGER NOT NULL,"
                                       "friend_id TEXT NOT NULL,"
                                       "message TEXT NOT NULL,"
                                       "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
                                       "FOREIGN KEY(user_id) REFERENCES users(id),"
                                       "FOREIGN KEY(friend_id) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql_create_table_dialogs, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(rc);
    }

    sqlite3_close(db);
}

int is_friend(sqlite3 *db, int user_id, int friend_id)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT user_id, friend_id FROM friends WHERE user_id = ? AND friend_id = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, friend_id);

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int get_user_id(sqlite3 *db, const char *login)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT id FROM users WHERE username = ?";
    int user_id = -1;

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return user_id;
    }

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        user_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return user_id;
}

int get_group_id(sqlite3 *db, const char *groupname)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT id FROM newgroups WHERE groupname = ?";
    int chat_id = -1;

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return chat_id;
    }

    sqlite3_bind_text(stmt, 1, groupname, -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        chat_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return chat_id;
}

char *get_username_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    const unsigned char *name = NULL;
    const char *sql = "SELECT username FROM users WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error SQL: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = sqlite3_column_text(stmt, 0);
        char *result = mx_strdup((const char *)name);
        sqlite3_finalize(stmt);
        return result;
    }

    sqlite3_finalize(stmt);
    return NULL;
}
