#include"../inc/server.h"

static int callback(void *data, int argc, char **argv, char **azColName) {//https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification
    (void)azColName;
    t_list **list_data = (t_list**)data;

    if (argc == 0)
        return 0;
    for (int i = 0; i < argc; i++) {
        if(argv[i] == NULL)
            mx_push_back(*(&list_data), strdup("NULL"));
        else
            mx_push_back(*(&list_data), strdup(argv[i]));
    }
    return 0;
}

void *sqlite3_exec_db(char *query, int type) {
    sqlite3 *db;
    int rc __attribute__((unused)) = sqlite3_open("server.db", &db);// ругается что не используем
    char *err_msg = 0;
    t_list *list = NULL;
    int auto_inc;
    
    if (type == 1) {
        rc = sqlite3_exec(db, query, callback, &list, &err_msg);//db,sql komanda vvoda,callback_dla kawdoi strochki,err_text;
        sqlite3_close(db);
        return list;
    }
    if (type == 2) {
        rc = sqlite3_exec(db, query, 0, 0, &err_msg);
        auto_inc = sqlite3_last_insert_rowid(db);//https://database.guide/how-last_insert_rowid-works-in-sqlite/
        sqlite3_close(db);
        int *p = &auto_inc;
        return p;
    }
    return NULL;
}
