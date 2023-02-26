#include "head_db.h"

int sql_open_db (sqlite3 **db){

    int rc = sqlite3_open("data_base/chat.db", db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        
        return 1;
    }
    return 0;
}

void sql_create_db () {
    sqlite3 *db;
    char *err_msg = 0;
    sql_open_db(&db);
    
    int rc;//table USER mit 4 colums
    char *sql_1 = "CREATE TABLE IF NOT EXISTS \"User\" ("
            "\"id\"	INTEGER NOT NULL UNIQUE,"
            "\"login\"	TEXT NOT NULL UNIQUE,"
            "\"password\"	TEXT NOT NULL,"
            "\"sslkey\"	TEXT,"
            "PRIMARY KEY(\"id\" AUTOINCREMENT))";
    
    rc = sqlite3_exec(db, sql_1, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return;
    } 
    
    sqlite3_close(db);
}

char *join(const char* s1, const char* s2) {
    char* result = malloc(strlen(s1) + strlen(s2) + 1);

    if (result) // thanks @pmg
    {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}
//find_var - from what colums // seek_var - name of fild we choose (condition)
//value_seek_var - value of condition on which we choose
//table - in what table we looking for
char* sql_get(char* find_var, char* seek_var, char *value_seek_var, int table) {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    sql_open_db(&db);
    char *sql = "SELECT ";
    sql = join(sql, find_var);
    switch (table)
    {
    case 1:// take table users
        sql = join(sql, " FROM User WHERE ");
        break;
    default:
        break;
    }
    sql = join(sql, seek_var);
    sql = join(sql, " = \"");
    sql = join(sql, value_seek_var);
    sql = join(sql, "\"");
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(res, 1, 3);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    char *answer = (char *)sqlite3_column_text(res, 0);
    if (answer) {
        char* result = malloc(strlen((const char*)answer));

        if (step == SQLITE_ROW)
        result = strcpy(result, (const char*)sqlite3_column_text(res, 0));    
        
        sqlite3_finalize(res);
        sqlite3_close(db);
        
        return result;
    }
    else {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return NULL;
    }
}
//for_mes
// static int callback(void *answ, int argc, char **argv, char **azColName){
//    int i;
//    azColName = 0;
//    for(i = 0; i<argc; i++){
//        char *value = strdup(argv[i]);
//       mx_push_back((t_list**)answ, (void*)value);
//    }
//    return 0;
// }


int sql_update(char *update_var, char *value_update_var, char *seek_var, char *value_seek_var, int table) {
    sqlite3 *db;
    char *err_msg = 0;
    sql_open_db(&db);
    char *sql = "UPDATE ";
    switch (table)
    {
    case 1:
        sql = join(sql, "User SET ");
        break;
    default:
        break;
    }
    sql = join(sql, update_var);
    sql = join(sql, " = \"");
    sql = join(sql, value_update_var);
    sql = join(sql, "\" WHERE ");
    sql = join(sql, seek_var);
    sql = join(sql, " = \"");
    sql = join(sql, value_seek_var);
    sql = join(sql, "\";");
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc == SQLITE_OK) return 0;
    else {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    }
}

int sql_delete(char* seek_var, char *value_seek_var, int table) {
    sqlite3 *db;
    sql_open_db(&db);
    char *sql = "DELETE";
    char *err_msg = 0;
    switch (table)
    {
    case 1:
        sql = join(sql, " FROM User WHERE ");
        break;
    default:
        break;
    }
    sql = join(sql, seek_var);
    sql = join(sql, " = \"");
    sql = join(sql, value_seek_var);
    sql = join(sql, "\"");
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc == SQLITE_OK) return 0;
    else {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    }
}
