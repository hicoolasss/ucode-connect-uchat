#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;

t_list *all_messages_list = NULL;

extern GtkWidget *chat_with_friend_grid;

// gboolean send_receive_msg(gpointer user_data)
// {

//     t_chat *message_data = (gpointer)user_data;

//     if (message_data)
//     {

//         t_chat *new_message = (t_chat *)malloc(sizeof(t_chat));
//         new_message->id = message_data->id;
//         new_message->sender = strdup(message_data->sender);
//         new_message->message = strdup(message_data->message);
//         new_message->timestamp = strdup(message_data->timestamp);

//         mx_push_back(&all_messages_list, new_message);

//         if (strcmp(message_data->sender, current_client.login) == 0)
//         {
//             const char *s_msg = message_data->message;

//             GtkWidget *sent_msg = gtk_label_new(s_msg);

//             gtk_widget_set_halign(sent_msg, GTK_ALIGN_END);

//             gtk_widget_set_margin_top(sent_msg, 15);

//             gtk_label_set_wrap(GTK_LABEL(sent_msg), TRUE);
//             gtk_label_set_wrap_mode(GTK_LABEL(sent_msg), PANGO_WRAP_WORD_CHAR);
//             gtk_label_set_max_width_chars(GTK_LABEL(sent_msg), 60);
//             gtk_label_set_selectable(GTK_LABEL(sent_msg), FALSE);

//             gtk_widget_set_hexpand(sent_msg, TRUE);

//             gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_msg, 0, message_data->id, 1, 1);

//             widget_styling(sent_msg, current_screen, "message");
//         }
//         else
//         {
//             const char *r_msg = message_data->message;

//             GtkWidget *received_msg = gtk_label_new(r_msg);

//             gtk_widget_set_halign(received_msg, GTK_ALIGN_START);

//             gtk_widget_set_margin_top(received_msg, 15);

//             gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
//             gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
//             gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 60);
//             gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

//             gtk_widget_set_hexpand(received_msg, TRUE);

//             gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_msg, 0, message_data->id, 1, 1);

//             widget_styling(received_msg, current_screen, "message");
//         }
//         return FALSE;
//     }
//     else
//     {
//         return FALSE;
//     }
// }

void add_message_to_chat_history(t_list **friend_list, const char *username, t_chat *new_chat)
{
        if (!friend_list || !new_chat)
    {
        return;
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
        friend_data->username = mx_strdup(username);
        friend_data->lastmessage = NULL;
        friend_data->chat_history = NULL;

        t_list *new_friend_node = (t_list *)malloc(sizeof(t_list));
        new_friend_node->data = friend_data;
        new_friend_node->next = *friend_list;
        *friend_list = new_friend_node;
    }

    // Добавить новую историю чата в найденном (или созданном) друге
    t_chat *chat_copy = (t_chat *)malloc(sizeof(t_chat));
    chat_copy->id = new_chat->id;
    chat_copy->sender = mx_strdup(new_chat->sender);
    chat_copy->message = mx_strdup(new_chat->message);
    chat_copy->timestamp = mx_strdup(new_chat->timestamp);

    mx_push_back(&friend_data->chat_history, chat_copy);

    // Обновить последнее сообщение для друга
    if (friend_data->lastmessage)
    {
        free(friend_data->lastmessage);
    }
    friend_data->lastmessage = mx_strdup(new_chat->message);

    t_list *friend_iter = *friend_list;

    while (friend_iter != NULL)
    {
        t_Friend *friend_data = (t_Friend *)friend_iter->data;

        printf("\n\nFriend: %s\n", friend_data->username);

        t_list *chat_history_iter = friend_data->chat_history;

        while (chat_history_iter != NULL)
        {
            t_chat *chat_data = (t_chat *)chat_history_iter->data;

            printf("  Message ID: %d\n", chat_data->id);
            printf("  Sender: %s\n", chat_data->sender);
            printf("  Message: %s\n", chat_data->message);
            printf("  Timestamp: %s\n", chat_data->timestamp);

            chat_history_iter = chat_history_iter->next;
        }

        friend_iter = friend_iter->next;
    }
    update_chat_history(friend_data);
}
