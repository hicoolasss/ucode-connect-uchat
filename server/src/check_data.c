#include "../inc/head_db.h"

int check_correct_login (char *login) {
    if (strlen(login) < 5 || strlen(login) > 10) return 1;
    for (int i = 0; i < mx_strlen(login); i++) {
        if (mx_isalpha(login[i]) || mx_isdigit(login[i]) || login[i] == '_') 
            continue;
        else return 1;
    }
    return 0;
}

int check_correct_password (char *password) {
    if (strlen(password) < 5 || strlen(password) > 10) return 1;
    char *correct = "_-@.*/";
    bool is_symbol = false;
    for (int i = 0; i < mx_strlen(password); i++) {
        if (strchr(correct, password[i]))
            is_symbol = true;
        else return 1;
    }
    return 0;

}
