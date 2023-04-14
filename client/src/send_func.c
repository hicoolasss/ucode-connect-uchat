#include "../inc/client.h"

void *send_func(void)
{
    while (running)
    {
        // Получаем команду из очереди (блокирующий вызов)
        pthread_mutex_lock(&command_queue_mutex);
        t_ThreadCommand *command = (t_ThreadCommand *)g_async_queue_pop(command_queue);
        pthread_mutex_unlock(&command_queue_mutex);

        // Обрабатываем команду в зависимости от command_type
        switch (command->command_type)
        {
        case COMMAND_TYPE_GET_USER_LIST:
            // Отправка запроса на сервер, чтобы получить список пользователей
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "login", current_client.login);
            cJSON_AddStringToObject(json, "command", "<user_list>");

            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            send_message_to_server(json_str);
            break;

            // Обработка других типов команд, если нужно

        default:
            break;
        }

        // Освобождаем память, выделенную для команды
        pthread_detach(pthread_self());
        g_free(command);
    }
}