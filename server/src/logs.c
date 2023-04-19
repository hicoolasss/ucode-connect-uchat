#include "../inc/server.h"

void write_logs(char* message)
{
    // Захватываем мьютекс
    pthread_mutex_lock(&logs_mutex);

    // Открываем файл на запись в конец
    FILE* log_file = fopen("server/logs/log.txt", "a");
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
