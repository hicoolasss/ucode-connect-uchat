#include "../inc/client.h"

gpointer send_func(gpointer data)
{
    pthread_mutex_lock(&mutex_recv);
    while (!main_client.connected)
    {
        pthread_cond_wait(&auth_cond, &mutex_recv);
    }
    // Здесь пользователь авторизован, и поток может продолжить работу с мьютексом
    pthread_mutex_unlock(&mutex_recv);
    while (running)
    {
        // Получаем команду из очереди (блокирующий вызов)
        t_ThreadCommand *command = (t_ThreadCommand *)g_async_queue_pop(command_queue);
        // Обрабатываем команду в зависимости от command_type
        switch (command->command_type)
        {
        case COMMAND_TYPE_GET_USER_LIST:
        {
            // Отправка запроса на сервер, чтобы получить список пользователей
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "login", current_client.login);
            cJSON_AddStringToObject(json, "command", "<user_list>");

            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            printf("Thread user_list in %lu trying to lock mutex\n", pthread_self());
            pthread_mutex_lock(&mutex_send);
            send_message_to_server(json_str);
            pthread_mutex_unlock(&mutex_send);
            printf("Thread user_list in %lu trying to unlock mutex\n", pthread_self());
            break;

            // Обработка других типов команд, если нужно
        }
        case COMMAND_TYPE_GET_FRIEND_LIST:
        {
            cJSON *json = cJSON_CreateObject();
            cJSON_AddStringToObject(json, "login", current_client.login);
            cJSON_AddStringToObject(json, "command", "<friend_list>");

            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            printf("Thread friend_list in %lu trying to lock mutex\n", pthread_self());
            pthread_mutex_lock(&mutex_send);
            send_message_to_server(json_str);
            pthread_mutex_unlock(&mutex_send);
            printf("Thread friend_list in %lu trying to unlock mutex\n", pthread_self());
            break;
        }
        case COMMAND_TYPE_SEND_MESSAGE:
        {
            cJSON *json = (cJSON *)g_async_queue_pop(message_queue);
            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            printf("Thread send_message in %lu trying to lock mutex\n", pthread_self());
            pthread_mutex_lock(&mutex_send);
            send_message_to_server(json_str);
            pthread_mutex_unlock(&mutex_send);
            printf("Thread send_message in %lu trying to unlock mutex\n", pthread_self());
            break;
        }
        case COMMAND_TYPE_GET_SHOW_HISTORY:
        {
            cJSON *json = (cJSON *)g_async_queue_pop(message_queue);
            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            printf("Thread show_history in %lu trying to lock mutex\n", pthread_self());
            pthread_mutex_lock(&mutex_send);
            send_message_to_server(json_str);
            pthread_mutex_unlock(&mutex_send);
            printf("Thread show_history in %lu trying to unlock mutex\n", pthread_self());
            break;
        }
        case COMMAND_TYPE_GET_ADD_FRIEND:
        {
            cJSON *json = (cJSON *)g_async_queue_pop(message_queue);
            char *json_str = cJSON_Print(json);
            cJSON_Delete(json);

            printf("Thread show_history in %lu trying to lock mutex\n", pthread_self());
            pthread_mutex_lock(&mutex_send);
            send_message_to_server(json_str);
            pthread_mutex_unlock(&mutex_send);
            printf("Thread show_history in %lu trying to unlock mutex\n", pthread_self());
            break;
        }
        default:
            break;
        }

        // Освобождаем память, выделенную для команды
    }
    // g_free(command);
    return NULL;
}
