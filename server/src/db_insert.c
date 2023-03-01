 #include "../inc/head_db.h"


int sql_set_user (char* login, char* password, char* sslkey){
    sqlite3 *db;
    char *err_msg = 0;
    
    sql_open_db(&db);
    char *sql = "INSERT INTO User(login, password, sslkey) VALUES (\"";
    sql = join(sql, login);
    sql = join(sql, "\", \"");
    sql = join(sql, password);
    sql = join(sql, "\", \"");
    sql = join(sql, sslkey);
    sql = join(sql, "\");");

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    return 0;
}
