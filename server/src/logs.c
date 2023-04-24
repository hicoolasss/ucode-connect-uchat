#include "../inc/server.h"

void write_logs(char *message)
{
    // Захватываем мьютекс
    pthread_mutex_lock(&logs_mutex);

    // Открываем файл на запись в конец
    FILE *log_file = fopen("server/logs/log.txt", "a");
    if (log_file == NULL)
    {
        // perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Записываем сообщение в файл
    fprintf(log_file, "%s\n", message);

    // Закрываем файл
    fclose(log_file);

    // Освобождаем мьютекс
    pthread_mutex_unlock(&logs_mutex);
}

void write_json_error(SSL *ssl, int cmd)
{
    if (ssl != NULL)
    {
        int error_code = SSL_get_error(ssl, cmd);
        char logs_buf[512];
        int logs_len = snprintf(logs_buf, sizeof(logs_buf), "Error sending command: %s\n", ERR_error_string(error_code, NULL));
        if (logs_len < (int)sizeof(logs_buf))
        {
            write_logs(logs_buf);
        }
        else
        {
            write_logs("Error: logs buffer too small\n");
        }
    }
    else
    {
        write_logs("Error: SSL connection is NULL\n");
    }
}
