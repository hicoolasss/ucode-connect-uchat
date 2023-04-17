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
        // printf("send mutex lock\n");
        // pthread_mutex_lock(&command_queue_mutex);
        cJSON *json = (cJSON *)g_async_queue_pop(message_queue);
        // pthread_mutex_unlock(&command_queue_mutex);
        // printf("send mutex unlock\n");
        char *json_str = cJSON_Print(json);
        send_message_to_server(json_str);
        // g_free(message_queue);
    }
    g_free(message_queue);
    return NULL;
}
