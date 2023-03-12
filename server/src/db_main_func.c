#include "../inc/head_db.h"

int db_log_to_serv(char *login, char *password, SSL *ssl)
{
    sql_create_db();
    char *log_id_check = sql_get("id", "login", login, 1);
    char *pass_id_check = sql_get("id", "password", password, 1);

    if ((log_id_check != NULL) && (pass_id_check != NULL))
    {
        int log_check = mx_atoi(log_id_check);
        int pass_check = mx_atoi(pass_id_check);

        if (log_check == pass_check)
        {
            mx_printstr("You are in chat(entered)!\n\n");
            return 0;
        }
    }
    else if ((log_id_check == NULL) && (pass_id_check == NULL)) {
        mx_printstr("user not found\n\n");
        return 1;
    }
    else if ((log_id_check != NULL) && (pass_id_check == NULL))
    {
        mx_printstr("incorrect password\n\n");
        return 2;
    }
    return 3;
}




int db_regestr_to_serv(char *login, char *password, SSL *ssl)
{
    sql_create_db();
    if (sql_set_user(login, password, ssl) == 1)
    {
        printf("this user already exist\n");
        return 1;
    }
    else
    {
        printf("You was regestered :-)\n\n");
        return 0;
    }


}

