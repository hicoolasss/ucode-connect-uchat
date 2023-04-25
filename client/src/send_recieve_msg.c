#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;

t_list *all_messages_list = NULL;

extern GtkWidget *chat_with_friend_grid;

t_list *mx_list_last(t_list *list) {
    if (!list) {
        return NULL;
    }

    while (list->next) {
        list = list->next;
    }

    return list;
}

t_list *add_message_to_chat_history(t_list **friend_list, const char *username, t_chat *new_chat)
{
    if (!friend_list || !new_chat)
    {
        return NULL;
    }

    t_Friend *friend_data = NULL;
    t_list *temp = *friend_list;

    // Найти друзей с заданным именем пользователя
    while (temp)
    {
        friend_data = (t_Friend *)temp->data;
        if (mx_strcmp(friend_data->username, username) == 0)
        {
            break;
        }
        temp = temp->next;
    }

    // Если друг с заданным именем пользователя не найден, создать нового друга и добавить его в friend_list
    if (!temp)
    {
        friend_data = (t_Friend *)malloc(sizeof(t_Friend));
        if (!friend_data)
        {
            return NULL;
        }

        friend_data->username = mx_strdup(username);
        friend_data->lastmessage = NULL;
        friend_data->chat_history = NULL;
        friend_data->in_chat = false;

        t_list *new_friend_node = (t_list *)malloc(sizeof(t_list));
        if (!new_friend_node)
        {
            free(friend_data);
            return NULL;
        }

        new_friend_node->data = friend_data;
        new_friend_node->next = *friend_list;
        *friend_list = new_friend_node;

        return new_friend_node;
    }

    // Добавить новую историю чата в найденном (или созданном) друге
    t_chat *chat_copy = (t_chat *)malloc(sizeof(t_chat));
    if (!chat_copy)
    {
        return NULL;
    }

    chat_copy->id = new_chat->id;
    chat_copy->sender = mx_strdup(new_chat->sender);
    chat_copy->message = mx_strdup(new_chat->message);
    chat_copy->timestamp = mx_strdup(new_chat->timestamp);

    pthread_mutex_lock(&mutex_send);
    mx_push_back(&friend_data->chat_history, chat_copy);
    pthread_mutex_unlock(&mutex_send);
    // Обновить последнее сообщение для друга
    friend_data->lastmessage = mx_strdup(new_chat->message);

    return friend_data->chat_history ? mx_list_last(friend_data->chat_history) : NULL;
}

void add_new_friend(t_list **friend_list, const char *username, const char *avatarname)
{
    if (!friend_list || !username)
        return;

    // Проверяем, есть ли уже друг с таким именем в списке
    t_list *temp = *friend_list;
    while (temp)
    {
        t_Friend *friend_data = (t_Friend *)temp->data;
        if (mx_strcmp(friend_data->username, username) == 0)
            return;
        temp = temp->next;
    }

    // Создаем новый объект для друга и добавляем его в начало списка
    t_Friend *friend_data = (t_Friend *)malloc(sizeof(t_Friend));
    if (!friend_data)
        return;

    friend_data->username = mx_strdup(username);
    friend_data->lastmessage = "Nothing here...";
    friend_data->chat_history = NULL;
    friend_data->in_chat = false;
    friend_data->avatarname = mx_strdup(avatarname);

    t_list *new_friend_node = (t_list *)malloc(sizeof(t_list));
    if (!new_friend_node)
    {
        free(friend_data);
        return;
    }

    new_friend_node->data = friend_data;
    new_friend_node->next = *friend_list;
    *friend_list = new_friend_node;

    update_show_chats_with_added_friends(*friend_list);
}
