#include "../inc/head_db.h"

int ent_sys(char *login, char *password, SSL *ssl)
{
    sql_create_db();
    // printf("Enter login...\n");
    // scanf("%10s[^\n]", login);
    // printf("Enter pass...\n");
    // scanf("%10s[^\n]", password);
    //"*" - поля таблицы которые хотим получить
    char *log_id_check = sql_get("id", "login", login, 1);
    char *pass_id_check = sql_get("id", "password", password, 1);

    if ((log_id_check != NULL) && (pass_id_check != NULL))
    {
        int log_check = mx_atoi(log_id_check);
        int pass_check = mx_atoi(pass_id_check);

        if (log_check == pass_check)
        {
            printf("You are in chat(entered)!\n\n");
        }
        else {
            printf("incorrect pass or login!\n\n");
            return 1;
        }
    }
    else if ((log_id_check != NULL) && (pass_id_check == NULL))
    {
        printf("incorrect password...\n\n");
        return 1;
    }
    else
    {
        sql_set_user(login, password, ssl);
        printf("You was regestered :-)\n\n");
    }
    return 0;
}
