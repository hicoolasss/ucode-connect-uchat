#include "../inc/client.h"

gpointer send_func()
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
        cJSON *json = (cJSON *)g_async_queue_pop(message_queue);
        char *json_str = cJSON_Print(json);
        send_message_to_server(json_str);
    }
    g_free(message_queue);
    return NULL;
}
