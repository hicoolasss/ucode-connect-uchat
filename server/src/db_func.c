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

    const char *sql_1 = "CREATE TABLE IF NOT EXISTS users ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "username TEXT NOT NULL UNIQUE,"
                        "password TEXT NOT NULL"
                        ");";

    rc = sqlite3_exec(db, sql_1, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
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
        return;
    }
    sqlite3_close(db);
}
