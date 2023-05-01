#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;

t_list *all_messages_list = NULL;

extern GtkWidget *chat_with_friend_grid;

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

    update_added_current_friend(*friend_list, username);

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
    friend_data->avatarname = g_strdup(avatarname);

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

void delete_message(t_list **friend_list, char *username, int message_id, char *message_text)
{
    t_list *friend_current = *friend_list;

    int id = message_id;

    const char *msg = message_text;

    while (friend_current != NULL)
    {
        t_Friend *friend = (t_Friend *)friend_current->data;

        if (strcmp(friend->username, username) == 0)
        {
            t_list *current = friend->chat_history;
            t_list *previous = NULL;

            while (current != NULL)
            {
                t_chat *message = (t_chat *)current->data;
                if (message->id == message_id && strcmp(message->message, message_text) == 0)
                {
                    //msg = g_strdup(message_text);
                    if (previous == NULL)
                    {
                        friend->chat_history = current->next;
                    }
                    else
                    {
                        previous->next = current->next;
                    }
                    free(message->message);
                    free(message);
                    free(current);
                    break;
                }

                previous = current;
                current = current->next;
            }
            break;
        }

        friend_current = friend_current->next;
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
    if (friend_data->in_chat)
    {
        //update_current_chat_while_delete(friend_data, id, msg);
        update_chat_history(friend_data);
    }


    t_list *current_chat = friend_data->chat_history;
    t_list *last_node = mx_list_last(current_chat);
    free(friend_data->lastmessage);
    friend_data->lastmessage = mx_strdup(((t_chat *)last_node->data)->message);
    //update_show_chats_with_added_friends(*friend_list, username);
    //update_current_show_chats_with_added_friends(friend_data, id, msg);
}

void update_message(t_list **friend_list, char *username, int old_message_id, char *new_message_text)
{
    t_list *friend_current = *friend_list;

    while (friend_current != NULL)
    {
        t_Friend *friend = (t_Friend *)friend_current->data;

        if (strcmp(friend->username, username) == 0)
        {
            t_list *current = friend->chat_history;

            while (current != NULL)
            {
                t_chat *message = (t_chat *)current->data;
                if (message->id == old_message_id)
                {
                    free(message->message);
                    message->message = strdup(new_message_text);
                    break;
                }

                current = current->next;
            }
            break;
        }

        friend_current = friend_current->next;
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
    if (friend_data->in_chat)
    {
        update_chat_history(friend_data);
    }
    t_list *current_chat = friend_data->chat_history;
    t_list *last_node = mx_list_last(current_chat);
    free(friend_data->lastmessage);
    friend_data->lastmessage = mx_strdup(((t_chat *)last_node->data)->message);
    //update_show_chats_with_added_friends(*friend_list, username);
}
