#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;

extern t_list *all_messages_list;

GdkPixbuf *scaled_avatar;

GtkWidget *user_list_grid;
GtkWidget *user_list_grid_scrolled;
GtkWidget *checkbox_btn;
GtkWidget *chat_with_friend_grid;

t_list *chat_history_temp;

int count = 0;
int temp_count = 0;

t_chat *find_last_message(t_list *message_list)
{

    if (message_list == NULL)
    {
        return NULL;
    }

    t_chat *last_message = (t_chat *)message_list->data;
    t_list *current = message_list->next;

    while (current)
    {
        t_chat *current_message = (t_chat *)current->data;

        // Сравниваем временные метки

        last_message = current_message;

        current = current->next;
    }

    return last_message;
}

static void get_scaled_image_chats()
{

    // GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(current_avatar.avatar, NULL);
    if (!current_avatar.avatar)
    {
        g_print("Ошибка при загрузке изображения.\n");
        return;
    }

    // Масштабирование исходного изображения до размера аватара
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(current_avatar.avatar, 60, 60, GDK_INTERP_BILINEAR);
    // g_object_unref(current_avatar.avatar);

    // Создание поверхности Cairo для рисования
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 60, 60);
    cairo_t *cr = cairo_create(surface);

    // Создание круглой области
    cairo_arc(cr, 60 / 2.0, 60 / 2.0, 60 / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    GdkPixbuf *circle_pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 60, 60);

    scaled_avatar = circle_pixbuf;
}
void create_new_chat(GtkToggleButton *toggle_button, gpointer user_data)
{

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<add_friend>");
    char *friend = mx_strdup(((t_user *)user_data)->username);
    cJSON_AddStringToObject(json, "friend", friend);

    g_async_queue_push(message_queue, json);

    cJSON *json1 = cJSON_CreateObject();
    cJSON_AddStringToObject(json1, "login", current_client.login);
    cJSON_AddStringToObject(json1, "command", "<friend_list>");
    g_async_queue_push(message_queue, json1);
}

void update_user_list_while_searching(GtkEditable *editable)
{
    GtkWidget *children, *iter;

    int pos = 0;

    children = gtk_widget_get_first_child(user_list_grid);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(user_list_grid))
    {
        gtk_widget_unparent(iter);
    }

    t_list *current = user_list;
    t_list *current_friend = friend_list;

    const gchar *entry_text = gtk_editable_get_text(editable);

    while (current != NULL)
    {
        gboolean should_continue = FALSE; // Add a flag to check if we should continue
        current_friend = friend_list;

        while (current_friend != NULL)
        {
            if (strcmp((((t_user *)current->data)->username), (((t_user *)current_friend->data)->username)) == 0)
            {
                should_continue = TRUE; // Set the flag to TRUE when the condition is met
                break;
            }
            else if (strcmp((((t_user *)current->data)->username), current_client.login) == 0)
            {
                should_continue = TRUE; // Set the flag to TRUE when the condition is met
                break;
            }
            current_friend = current_friend->next;
        }

        if (should_continue) // Check the flag and continue the outer loop if necessary
        {
            current = current->next;
            continue;
        }

        if (strncmp(entry_text, (((t_user *)current->data)->username), strlen(entry_text)) == 0)
        {
            pos++;

            GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

            GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

            GtkWidget *username_label = gtk_label_new(((t_user *)current->data)->username);

            checkbox_btn = gtk_check_button_new();

            gtk_widget_set_hexpand(username_label, TRUE);

            gtk_widget_set_halign(checkbox_btn, GTK_ALIGN_END);

            gtk_widget_set_halign(username_label, GTK_ALIGN_START);

            gtk_box_append(GTK_BOX(user_info_box), user_avatar);

            gtk_box_append(GTK_BOX(user_info_box), username_label);

            gtk_box_append(GTK_BOX(user_info_box), checkbox_btn);

            gtk_widget_set_margin_start(user_info_box, 28);
            gtk_widget_set_margin_end(user_info_box, 22);
            gtk_widget_set_margin_top(user_info_box, 20);

            if (current->next == NULL)
            {
                gtk_widget_set_margin_bottom(user_info_box, 20);
            }

            gtk_widget_set_size_request(user_info_box, 400, 60);

            gtk_widget_set_halign(user_info_box, GTK_ALIGN_CENTER);

            gtk_widget_set_margin_start(user_avatar, 0);
            gtk_widget_set_margin_top(user_avatar, 0);
            gtk_widget_set_margin_bottom(user_avatar, 0);

            gtk_widget_set_size_request(user_avatar, 60, 60);

            gtk_widget_set_margin_start(username_label, 8);
            gtk_widget_set_margin_top(username_label, 4);
            gtk_widget_set_margin_bottom(username_label, 14);

            gtk_widget_set_margin_top(checkbox_btn, 12);
            gtk_widget_set_margin_bottom(checkbox_btn, 13);

            // gtk_widget_set_size_request(checkbox_btn, 30, 30);

            gtk_grid_attach(GTK_GRID(user_list_grid), user_info_box, 0, pos, 1, 1);

            g_signal_connect_data(checkbox_btn, "toggled", G_CALLBACK(create_new_chat), current->data, NULL, 0);

            widget_styling(user_info_box, current_screen, "user_info_box");
        }

        current = current->next;
    }
}

void show_user_list_scrolled(t_list *current)
{

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(user_list_grid);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(user_list_grid))
    {

        gtk_widget_unparent(iter);
    }

    int pos = 0;

    get_scaled_image_chats();
    t_list *current_friend = friend_list;
    while (current != NULL)
    {
        gboolean should_continue = FALSE; // Add a flag to check if we should continue
        current_friend = friend_list;

        while (current_friend != NULL)
        {
            if (strcmp((((t_user *)current->data)->username), (((t_user *)current_friend->data)->username)) == 0)
            {
                should_continue = TRUE; // Set the flag to TRUE when the condition is met
                break;
            }
            else if (strcmp((((t_user *)current->data)->username), current_client.login) == 0)
            {
                should_continue = TRUE; // Set the flag to TRUE when the condition is met
                break;
            }
            current_friend = current_friend->next;
        }

        if (should_continue) // Check the flag and continue the outer loop if necessary
        {
            current = current->next;
            continue;
        }
        pos++;
        // mx_printstr(((t_user*)current->data)->username);
        GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

        GtkWidget *username_label = gtk_label_new(((t_user *)current->data)->username);

        checkbox_btn = gtk_check_button_new();

        gtk_widget_set_hexpand(username_label, TRUE);

        // gtk_widget_set_hexpand(user_info_box, TRUE);

        gtk_widget_set_halign(checkbox_btn, GTK_ALIGN_END);

        gtk_widget_set_halign(username_label, GTK_ALIGN_START);

        gtk_box_append(GTK_BOX(user_info_box), user_avatar);

        gtk_box_append(GTK_BOX(user_info_box), username_label);

        gtk_box_append(GTK_BOX(user_info_box), checkbox_btn);

        gtk_widget_set_margin_start(user_info_box, 28);
        gtk_widget_set_margin_end(user_info_box, 22);
        gtk_widget_set_margin_top(user_info_box, 20);

        if (current->next == NULL)
        {
            gtk_widget_set_margin_bottom(user_info_box, 20);
        }

        gtk_widget_set_size_request(user_info_box, 400, 60);

        gtk_widget_set_halign(user_info_box, GTK_ALIGN_CENTER);

        gtk_widget_set_margin_start(user_avatar, 0);
        gtk_widget_set_margin_top(user_avatar, 0);
        gtk_widget_set_margin_bottom(user_avatar, 0);

        gtk_widget_set_size_request(user_avatar, 60, 60);

        gtk_widget_set_margin_start(username_label, 8);
        gtk_widget_set_margin_top(username_label, 4);
        gtk_widget_set_margin_bottom(username_label, 14);

        gtk_widget_set_margin_top(checkbox_btn, 12);
        gtk_widget_set_margin_bottom(checkbox_btn, 13);

        // gtk_widget_set_size_request(checkbox_btn, 30, 30);

        gtk_grid_attach(GTK_GRID(user_list_grid), user_info_box, 0, pos, 1, 1);

        g_signal_connect_data(checkbox_btn, "toggled", G_CALLBACK(create_new_chat), current->data, NULL, 0);

        widget_styling(user_info_box, current_screen, "user_info_box");

        current = current->next;
    }
}

static void on_entry_activate(GtkEntry *entry, gpointer friend_data)
{

    t_Friend *data = friend_data;

    GtkEntryBuffer *text = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char *username = strdup(data->username);
    const char *message = g_strdup(gtk_entry_buffer_get_text(text));
    // Теперь переменная text_copy содержит текст из виджета GtkEntry
    // Вы можете использовать text_copy для дальнейших действий, например, отправки сообщения

    printf("%s -> %s\n", username, message);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<send_message>");
    cJSON_AddStringToObject(json, "friend", username);
    cJSON_AddStringToObject(json, "message", message);

    g_async_queue_push(message_queue, json);

    //g_signal_connect(entry, "activate", G_CALLBACK(update_chat_history), data);

    gtk_editable_set_text(GTK_EDITABLE(entry), "");
}

void update_chat_history(gpointer friend_data) {

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.chat_with_friend);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.chat_with_friend))
    {

        gtk_widget_unparent(iter);
    }

    chat_with_friend_grid = create_grid(607, 607, "empty");

    GtkWidget *chat_with_friend_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), chat_with_friend_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    int last_child = 0;

    t_Friend *friend_iter = friend_data;

    if (friend_iter->chat_history)
    {

        t_list *chat_history_iter = friend_iter->chat_history;

        while (chat_history_iter != NULL)
        {

            t_chat *chat_data = (t_chat *)chat_history_iter->data;

            int pos = chat_data->id;

            if (strcmp(chat_data->sender, current_client.login) == 0)
            {
                const char *s_msg = chat_data->message;

                GtkWidget *sent_msg = gtk_label_new(s_msg);

                gtk_widget_set_halign(sent_msg, GTK_ALIGN_END);

                gtk_widget_set_margin_top(sent_msg, 15);

                gtk_label_set_wrap(GTK_LABEL(sent_msg), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(sent_msg), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(sent_msg), 60);
                gtk_label_set_selectable(GTK_LABEL(sent_msg), FALSE);

                gtk_widget_set_hexpand(sent_msg, TRUE);

                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_msg, 0, pos, 1, 1);

                widget_styling(sent_msg, current_screen, "message");
            }
            else
            {

                const char *r_msg = chat_data->message;

                GtkWidget *received_msg = gtk_label_new(r_msg);

                gtk_widget_set_halign(received_msg, GTK_ALIGN_START);

                gtk_widget_set_margin_top(received_msg, 15);

                gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 60);
                gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

                gtk_widget_set_hexpand(received_msg, TRUE);

                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_msg, 0, pos, 1, 1);

                widget_styling(received_msg, current_screen, "message");
            }

            chat_history_iter = chat_history_iter->next;

            last_child++;
        
        }

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        GtkWidget *entry = gtk_entry_new();

        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

        gtk_box_append(GTK_BOX(box), entry);

        gtk_widget_set_margin_start(entry, 17);
        gtk_widget_set_margin_end(entry, 17);
        gtk_widget_set_margin_top(entry, 14);
        gtk_widget_set_margin_bottom(entry, 14);

        gtk_widget_set_margin_start(box, 26);
        gtk_widget_set_margin_end(box, 60);
        gtk_widget_set_margin_top(box, 10);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_widget_set_margin_top(chat_with_friend_scrolled, 75);
        // gtk_widget_set_margin_bottom(chat_with_friend_scrolled, 16);
        gtk_widget_set_size_request(chat_with_friend_scrolled, 533, 607);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), chat_with_friend_scrolled, 0, 0, 1, 1);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 9999, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), friend_iter);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");
    }
    else
    {

        mx_printstr("empty chat with : ");
        mx_printstr(friend_iter->username);
        mx_printstr("\n");
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        GtkWidget *entry = gtk_entry_new();

        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

        gtk_box_append(GTK_BOX(box), entry);

        gtk_widget_set_margin_start(entry, 17);
        gtk_widget_set_margin_end(entry, 17);
        gtk_widget_set_margin_top(entry, 14);
        gtk_widget_set_margin_bottom(entry, 14);

        gtk_widget_set_margin_start(box, 26);
        gtk_widget_set_margin_end(box, 60);
        gtk_widget_set_margin_top(box, 613);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 0, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), friend_iter);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");

    }

}

void show_chat_with_friend(GtkWidget *btn, gpointer friend_data)
{
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.empty_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), TRUE);

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.chat_with_friend);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.chat_with_friend))
    {

        gtk_widget_unparent(iter);
    }

    chat_with_friend_grid = create_grid(607, 607, "empty");

    GtkWidget *chat_with_friend_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), chat_with_friend_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    int last_child = 0;

    t_Friend *friend_iter = friend_data;

    if (friend_iter->chat_history)
    {

        t_list *chat_history_iter = friend_iter->chat_history;

        while (chat_history_iter != NULL)
        {

            t_chat *chat_data = (t_chat *)chat_history_iter->data;

            int pos = chat_data->id;

            if (strcmp(chat_data->sender, current_client.login) == 0)
            {
                const char *s_msg = chat_data->message;

                GtkWidget *sent_msg = gtk_label_new(s_msg);

                gtk_widget_set_halign(sent_msg, GTK_ALIGN_END);

                gtk_widget_set_margin_top(sent_msg, 15);

                gtk_label_set_wrap(GTK_LABEL(sent_msg), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(sent_msg), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(sent_msg), 60);
                gtk_label_set_selectable(GTK_LABEL(sent_msg), FALSE);

                gtk_widget_set_hexpand(sent_msg, TRUE);

                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_msg, 0, pos, 1, 1);

                widget_styling(sent_msg, current_screen, "message");
            }
            else
            {

                const char *r_msg = chat_data->message;

                GtkWidget *received_msg = gtk_label_new(r_msg);

                gtk_widget_set_halign(received_msg, GTK_ALIGN_START);

                gtk_widget_set_margin_top(received_msg, 15);

                gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 60);
                gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

                gtk_widget_set_hexpand(received_msg, TRUE);

                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_msg, 0, pos, 1, 1);

                widget_styling(received_msg, current_screen, "message");
            }

            chat_history_iter = chat_history_iter->next;

            last_child++;
        
        }

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        GtkWidget *entry = gtk_entry_new();

        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

        gtk_box_append(GTK_BOX(box), entry);

        gtk_widget_set_margin_start(entry, 17);
        gtk_widget_set_margin_end(entry, 17);
        gtk_widget_set_margin_top(entry, 14);
        gtk_widget_set_margin_bottom(entry, 14);

        gtk_widget_set_margin_start(box, 26);
        gtk_widget_set_margin_end(box, 60);
        gtk_widget_set_margin_top(box, 10);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_widget_set_margin_top(chat_with_friend_scrolled, 75);
        // gtk_widget_set_margin_bottom(chat_with_friend_scrolled, 16);
        gtk_widget_set_size_request(chat_with_friend_scrolled, 533, 607);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), chat_with_friend_scrolled, 0, 0, 1, 1);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 9999, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), friend_iter);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");
    }
    else
    {

        mx_printstr("empty chat with : ");
        mx_printstr(friend_iter->username);
        mx_printstr("\n");
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        GtkWidget *entry = gtk_entry_new();

        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

        gtk_box_append(GTK_BOX(box), entry);

        gtk_widget_set_margin_start(entry, 17);
        gtk_widget_set_margin_end(entry, 17);
        gtk_widget_set_margin_top(entry, 14);
        gtk_widget_set_margin_bottom(entry, 14);

        gtk_widget_set_margin_start(box, 26);
        gtk_widget_set_margin_end(box, 60);
        gtk_widget_set_margin_top(box, 613);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 0, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), friend_iter);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");

    }
}

void show_chats_with_added_friends(t_list *friend_list)
{

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.chats_list_grid_child);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.chats_list_grid_child))
    {
        gtk_widget_unparent(iter);
    }

    t_list *current = friend_list;

    while (current != NULL)
    {
        t_Friend *friend_data = (t_Friend *)current->data;

        GtkWidget *user_box_btn1 = gtk_button_new();

        GtkWidget *user_box_grid = create_grid(400, 80, NULL);

        GtkWidget *username_label = gtk_label_new(friend_data->username);

        get_scaled_image_chats();

        GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

        // t_chat *last_element = find_last_message(current);
        GtkWidget *last_msg_label = gtk_label_new(friend_data->lastmessage);
        // }
        // if (friend_data->lastmessage == NULL)
        // {
        //     gtk_label_set_label(GTK_LABEL(last_msg_label), "Nothing here...");
        // }

        gtk_grid_attach(GTK_GRID(user_box_grid), user_avatar, 0, 0, 1, 2);

        gtk_grid_attach(GTK_GRID(user_box_grid), username_label, 1, 0, 1, 1);

        gtk_grid_attach(GTK_GRID(user_box_grid), last_msg_label, 1, 1, 1, 1);

        gtk_button_set_child(GTK_BUTTON(user_box_btn1), user_box_grid);

        gtk_widget_set_halign(user_box_btn1, GTK_ALIGN_CENTER);
        gtk_widget_set_hexpand(user_box_btn1, TRUE);
        gtk_widget_set_vexpand(user_box_btn1, FALSE);
        gtk_widget_set_vexpand(user_box_grid, FALSE);

        gtk_widget_set_margin_top(user_box_btn1, 5);
        gtk_widget_set_margin_bottom(user_box_btn1, 10);

        gtk_widget_set_size_request(user_box_btn1, 400, 80);

        gtk_widget_set_margin_start(user_avatar, 6);
        gtk_widget_set_margin_top(user_avatar, 10);
        gtk_widget_set_margin_bottom(user_avatar, 10);

        gtk_widget_set_halign(username_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(username_label, 5);
        // gtk_widget_set_margin_bottom(username_label, 0);

        // gtk_widget_set_hexpand(last_msg_label, TRUE);

        gtk_widget_set_valign(last_msg_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(last_msg_label, 5);

        gtk_widget_set_size_request(user_avatar, 60, 60);

        // gtk_widget_set_size_request(current_grid.chats_list_grid_child, 427, 246);

        // gtk_widget_set_hexpand(current_grid.chats_list_grid_child, FALSE);

        gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn1, 0, count, 1, 1);
        count++;

        gpointer username_copy = (gpointer)friend_data->username;

        g_signal_connect(user_box_btn1, "clicked", G_CALLBACK(show_chat_with_friend), friend_data);

        widget_styling(user_box_btn1, current_screen, "user_box_btn");

        widget_styling(username_label, current_screen, "username_label_in_chats");

        widget_styling(last_msg_label, current_screen, "last_msg_label");

        current = current->next;
    }
}

void show_create_new_chat_with_someone()
{

    user_list_grid = create_grid(451, 227, "mini_chats");

    user_list_grid_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(user_list_grid_scrolled), user_list_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(user_list_grid_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    GtkWidget *create_new_chat_with_someone_label = gtk_label_new("Create new chat...");

    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), "Search friend...");

    gtk_grid_attach(GTK_GRID(current_grid.chats), create_new_chat_with_someone_label, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.chats), entry_for_search, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.chats), user_list_grid_scrolled, 0, 2, 1, 1);

    gtk_widget_set_margin_start(create_new_chat_with_someone_label, 175);
    gtk_widget_set_margin_end(create_new_chat_with_someone_label, 175);
    gtk_widget_set_margin_top(create_new_chat_with_someone_label, 83);

    gtk_widget_set_margin_start(user_list_grid_scrolled, 53);
    gtk_widget_set_margin_end(user_list_grid_scrolled, 53);
    gtk_widget_set_margin_top(user_list_grid_scrolled, 49);

    gtk_widget_set_size_request(user_list_grid_scrolled, 451, 227);

    gtk_widget_set_halign(user_list_grid_scrolled, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(entry_for_search, 128);
    gtk_widget_set_margin_end(entry_for_search, 129);
    gtk_widget_set_margin_top(entry_for_search, 47);

    gtk_widget_set_size_request(entry_for_search, 300, 45);

    widget_styling(create_new_chat_with_someone_label, current_screen, "create_new_chat_with_someone_label");
    widget_styling(entry_for_search, current_screen, "entry_for_search_user");

    g_signal_connect(entry_for_search, "changed", G_CALLBACK(update_user_list_while_searching), NULL);
}
