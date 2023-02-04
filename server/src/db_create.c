#include "../inc/server.h"

void sqlite3_create_db() {
    struct stat buffer;
    int exist = stat("server.db", &buffer);
    if (exist != 0) {
        sqlite3 *db;
        int rc = sqlite3_open("server.db", &db);
        char *sql = NULL, *err_msg = 0;
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }
        sql = mx_strrejoin(sql, "PRAGMA encoding = \"UTF-8\";");
        sql = mx_strrejoin(sql, "CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT NOT NULL, password TEXT NOT NULL, name TEXT DEFAULT \".clear\", surname TEXT DEFAULT \".clear\", bio TEXT DEFAULT \".clear\", avatar TEXT DEFAULT \"default\", theme TEXT DEFAULT dark);");
        sql = mx_strrejoin(sql, "CREATE TABLE chats (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, members INTEGER NOT NULL, avatar TEXT DEFAULT \"default\");");
        sql = mx_strrejoin(sql, "CREATE TABLE members (id INTEGER PRIMARY KEY AUTOINCREMENT, chat_id INTEGER NOT NULL, user_id INTEGER NOT NULL, admin BOOLEAN NOT NULL DEFAULT FALSE, mute BOOLEAN NOT NULL DEFAULT FALSE);");
        sql = mx_strrejoin(sql, "CREATE TABLE messages (id INTEGER PRIMARY KEY AUTOINCREMENT, chat_id INTEGER NOT NULL, user_id INTEGER NOT NULL, text TEXT DEFAULT NULL, time DATETIME NOT NULL, type TEXT DEFAULT text);");
        sql = mx_strrejoin(sql, "INSERT INTO users (login, password) VALUES ('leva', 'levapass');");
        sql = mx_strrejoin(sql, "INSERT INTO users (login, password) VALUES ('vasia', 'vasiapass');");
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to select data\n");
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }
        mx_strdel(&sql);
        sqlite3_close(db);
    }
}
