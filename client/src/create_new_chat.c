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
GtkWidget *chat_with_friend_scrolled;

GtkWidget *entry;

t_Friend *current_friend = NULL;

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

static void get_scaled_image_chats(t_list *current)
{

    GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(((t_user *)current->data)->avatarname, NULL);
    // mx_printstr(((t_user *)current->data)->avatarname);
    if (!source_pixbuf)
    {
        g_print("Ошибка при загрузке изображения.3\n");
        return;
    }

    // Масштабирование исходного изображения до размера аватара
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(source_pixbuf, 60, 60, GDK_INTERP_BILINEAR);
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

const char *format_time(const char *timestamp)
{
    struct tm time_struct;
    strptime(timestamp, "%Y-%m-%d %H:%M:%S", &time_struct);

    static char formatted_time[6];
    strftime(formatted_time, sizeof(formatted_time), "%H:%M", &time_struct);

    return formatted_time;
}

static void get_scaled_image_chats_for_friend_list(t_Friend *current)
{

    GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(current->avatarname, NULL);
    // mx_printstr(((t_user *)current->data)->avatarname);
    if (!source_pixbuf)
    {
        g_print("Ошибка при загрузке изображения.3\n");
        return;
    }

    // Масштабирование исходного изображения до размера аватара
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(source_pixbuf, 60, 60, GDK_INTERP_BILINEAR);
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

    (void)toggle_button;
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<add_friend>");
    char *friend = mx_strdup(((t_user *)user_data)->username);
    cJSON_AddStringToObject(json, "friend", friend);

    g_async_queue_push(message_queue, json);
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

    get_scaled_image_chats(current);
    t_list *current_friend = friend_list;
    while (current != NULL)
    {
        gboolean should_continue = FALSE; // Add a flag to check if we should continue
        current_friend = friend_list;

        // Only check if current_friend is empty and set the flag to TRUE if it is
        if (current_friend == NULL)
        {

            if (strcmp((((t_user *)current->data)->username), current_client.login) == 0)
            {
                should_continue = TRUE; // Set the flag to TRUE when the condition is met
            }
        }
        else
        {
            while (current_friend != NULL) // Remove !should_continue from the while condition
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
        }

        if (should_continue) // Check the flag and continue the outer loop if necessary
        {
            current = current->next;
            continue;
        }

        pos++;
        // mx_printstr(((t_user*)current->data)->username);

        GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        get_scaled_image_chats(current);

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

    gboolean editing_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(entry), "editing_mode"));

    if (editing_mode)
    {
        return;
    }

    current_achievements.first_step = true;
    update_show_achievements();

    SentMessageData *sent_message_data = (SentMessageData *)friend_data;
    mx_printstr("otpravka\n");

    t_Friend *data = sent_message_data->friend_data;

    const char *username = strdup(data->username);
    const char *message = gtk_editable_get_text(GTK_EDITABLE(entry));

    if (strcmp(message, "I love you, baby!") == 0)
    {
        current_achievements.loving = true;
        update_show_achievements();
    }

    // printf("%s -> %s\n", username, message);
    if (message == NULL)
    {
        return;
    }
    // Игнорировать начальные и конечные пробелы
    while (isspace(*message))
    {
        message++;
    }
    if (*message == '\0')
    {
        return;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<send_message_in_chat>");
    cJSON_AddStringToObject(json, "friend", username);
    cJSON_AddStringToObject(json, "message", message);

    g_async_queue_push(message_queue, json);

    gtk_editable_set_text(GTK_EDITABLE(entry), "");
}

static void on_entry_activate_for_editing(GtkEntry *entry, gpointer user_data)
{

    gboolean editing_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(entry), "editing_mode"));

    if (!editing_mode)
    {
        return;
    }

    mx_printstr("on_entry_activate_for_editing\n");

    SentMessageData *sent_message_data = (SentMessageData *)user_data;
    t_chat *chat_data = sent_message_data->chat_data;
    t_Friend *friend_data = sent_message_data->friend_data;

    const char *username = NULL;

    if (friend_data)
    {
        username = g_strdup(friend_data->username);
    }
    else
    {
        username = g_strdup(sent_message_data->friendname);
    }
    char *sender = chat_data->sender;
    int message_id = chat_data->id;
    char *old_message = chat_data->message;

    const char *new_message = gtk_editable_get_text(GTK_EDITABLE(entry));

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<update_message_in_chat>");
    cJSON_AddStringToObject(json, "friendname", username);
    cJSON_AddStringToObject(json, "sender", sender);
    cJSON_AddStringToObject(json, "old_message", old_message);
    cJSON_AddStringToObject(json, "new_message", new_message);
    cJSON_AddNumberToObject(json, "message_id", message_id);

    g_async_queue_push(message_queue, json);

    gtk_editable_set_text(GTK_EDITABLE(entry), "");

    g_signal_handlers_disconnect_by_func(entry, (gpointer)G_CALLBACK(on_entry_activate_for_editing), user_data);

    // Включаем обработчик сигнала для отправки сообщения
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), user_data);

    g_object_set_data(G_OBJECT(entry), "editing_mode", GINT_TO_POINTER(FALSE));
}

static gboolean scroll_to_bottom(gpointer user_data)
{
    GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(user_data);
    GtkAdjustment *v_adjustment = gtk_scrolled_window_get_vadjustment(scrolled_window);
    double upper = gtk_adjustment_get_upper(v_adjustment);
    double page_size = gtk_adjustment_get_page_size(v_adjustment);
    gtk_adjustment_set_value(v_adjustment, upper - page_size);

    return FALSE; // Возвращаем FALSE, чтобы удалить функцию из очереди выполнения
}

static void on_delete_msg_clicked(GtkWidget *btn, gpointer user_data)
{
    (void)btn;

    current_achievements.secretive = true;
    update_show_achievements();

    SentMessageData *sent_message_data = (SentMessageData *)user_data;
    t_chat *chat_data = sent_message_data->chat_data;
    t_Friend *friend_data = sent_message_data->friend_data;

    char *username = NULL;

    if (friend_data)
    {
        username = g_strdup(friend_data->username);
    }
    else
    {
        username = g_strdup(sent_message_data->friendname);
    }
    char *sender = chat_data->sender;
    char *message = chat_data->message;
    int message_id = chat_data->id;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<delete_message_in_chat>");
    cJSON_AddStringToObject(json, "friendname", username);
    cJSON_AddStringToObject(json, "sender", sender);
    cJSON_AddStringToObject(json, "message", message);
    cJSON_AddNumberToObject(json, "message_id", message_id);

    g_async_queue_push(message_queue, json);
}

static void on_edit_msg_clicked(GtkWidget *btn, gpointer user_data)
{
    (void)btn;
    mx_printstr("on_edit_msg_clicked\n");

    SentMessageData *sent_message_data = (SentMessageData *)user_data;
    t_chat *chat_data = sent_message_data->chat_data;
    // t_Friend *friend_data = sent_message_data->friend_data;

    g_object_set_data(G_OBJECT(entry), "editing_mode", GINT_TO_POINTER(TRUE));

    gtk_editable_set_text(GTK_EDITABLE(entry), chat_data->message);

    // Отключаем обработчик сигнала для отправки сообщения
    g_signal_handlers_disconnect_by_func(entry, (gpointer)G_CALLBACK(on_entry_activate), sent_message_data);

    // Подключаем обработчик сигнала для редактирования сообщения
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate_for_editing), sent_message_data);
}

static void on_cancel_btn_clicked(GtkWidget *btn, gpointer user_data)
{
    (void)btn;
    mx_printstr("on_cancel_btn_clicked\n");

    SentMessageData *sent_message_data = (SentMessageData *)user_data;
    GtkWidget *box = sent_message_data->sent_box;

    gtk_widget_unparent(box);

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(box);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(box))
    {
        gtk_widget_unparent(iter);
    }

    const char *s_msg = sent_message_data->chat_data->message;

    const char *s_msg_time = format_time(sent_message_data->chat_data->timestamp);

    GtkWidget *sent_msg = gtk_button_new_with_label(s_msg);

    GtkWidget *sent_time = gtk_label_new(s_msg_time);

    gtk_box_append(GTK_BOX(box), sent_time);
    gtk_box_append(GTK_BOX(box), sent_msg);

    widget_styling(sent_msg, current_screen, "sent_message");
    widget_styling(sent_time, current_screen, "time");

    // Прикрепление виджета к новому контейнеру
    gtk_grid_attach(GTK_GRID(chat_with_friend_grid), box, 1, 9999 + sent_message_data->chat_data->id, 1, 1);

    g_signal_connect(sent_msg, "clicked", G_CALLBACK(on_sent_msg_clicked), user_data);
}

void on_sent_msg_clicked(GtkWidget *btn, gpointer user_data)
{
    (void)btn;
    SentMessageData *sent_message_data = (SentMessageData *)user_data;
    g_object_set_data(G_OBJECT(entry), "editing_mode", GINT_TO_POINTER(FALSE));

    // t_Friend *friend_data = sent_message_data->friend_data;

    GtkWidget *box = sent_message_data->sent_box;

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(box);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(box))
    {
        gtk_widget_unparent(iter);
    }

    // const char *msg = gtk_button_get_label(GTK_BUTTON(btn));

    GtkWidget *edit_btn = gtk_button_new();

    GtkWidget *delete_btn = gtk_button_new();

    GtkWidget *cancel_btn = gtk_button_new();

    gtk_box_append(GTK_BOX(box), edit_btn);
    gtk_box_append(GTK_BOX(box), delete_btn);
    gtk_box_append(GTK_BOX(box), cancel_btn);

    gtk_widget_set_size_request(edit_btn, 16, 16);
    gtk_widget_set_size_request(delete_btn, 16, 16);
    gtk_widget_set_size_request(cancel_btn, 16, 16);

    widget_styling(edit_btn, current_screen, "edit_btn");
    widget_styling(delete_btn, current_screen, "delete_btn");
    widget_styling(cancel_btn, current_screen, "cancel_btn");

    g_signal_connect(delete_btn, "clicked", G_CALLBACK(on_delete_msg_clicked), user_data);

    g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_msg_clicked), user_data);

    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_cancel_btn_clicked), user_data);

    gtk_widget_grab_focus(entry);
}

void update_chat_history(gpointer friend_data)
{
    pthread_mutex_lock(&mutex_send);

    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), FALSE);
    // gtk_widget_set_visible(GTK_WIDGET(current_grid.empty_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), TRUE);

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(chat_with_friend_grid);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(chat_with_friend_grid))
    {
        gtk_widget_unparent(iter);
    }

    GtkWidget *transparent_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(transparent_widget, FALSE);
    gtk_widget_set_vexpand(transparent_widget, TRUE);
    gtk_grid_attach(GTK_GRID(chat_with_friend_grid), transparent_widget, 0, 0, 1, 1);

    t_Friend *friend_iter = friend_data;

    // show_friend_info(friend_data);

    mx_printstr("update chat with : ");
    mx_printstr(friend_iter->username);
    mx_printstr("\n");

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

                const char *s_msg_time = format_time(chat_data->timestamp);

                GtkWidget *sent_msg = gtk_button_new_with_label(s_msg);

                GtkWidget *sent_time = gtk_label_new(s_msg_time);

                GtkWidget *edit_btn = gtk_button_new();

                GtkWidget *delete_btn = gtk_button_new();

                GtkLabel *sent_msg_label = GTK_LABEL(gtk_button_get_child(GTK_BUTTON(sent_msg)));

                gtk_label_set_wrap(GTK_LABEL(sent_msg_label), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(sent_msg_label), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(sent_msg_label), 30);
                gtk_label_set_selectable(GTK_LABEL(sent_msg_label), FALSE);

                gtk_widget_set_hexpand(sent_msg, TRUE);

                gtk_widget_set_halign(sent_msg, GTK_ALIGN_END); // Выравнивание received_msg по левому краю

                gtk_widget_set_halign(sent_time, GTK_ALIGN_START); // Выравнивание received_time по левому краю

                GtkWidget *sent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
                gtk_box_append(GTK_BOX(sent_box), sent_time);
                gtk_box_append(GTK_BOX(sent_box), sent_msg);
                // gtk_box_append(GTK_BOX(sent_box), edit_btn);
                // gtk_box_append(GTK_BOX(sent_box), delete_btn);

                gtk_widget_set_size_request(edit_btn, 16, 16);
                gtk_widget_set_size_request(delete_btn, 16, 16);

                gtk_widget_set_margin_top(sent_box, 15);

                gtk_widget_set_halign(sent_box, GTK_ALIGN_END);
                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_box, 1, 9999 + pos, 1, 1);

                widget_styling(sent_msg, current_screen, "sent_message");
                widget_styling(sent_time, current_screen, "time");
                widget_styling(edit_btn, current_screen, "edit_btn");
                widget_styling(delete_btn, current_screen, "delete_btn");

                SentMessageData *sent_message_data = malloc(sizeof(SentMessageData));
                sent_message_data->chat_data = chat_data;
                sent_message_data->sent_box = sent_box;
                sent_message_data->friend_data = friend_iter;

                g_signal_connect(sent_msg, "clicked", G_CALLBACK(on_sent_msg_clicked), sent_message_data);
            }
            else
            {
                const char *r_msg = chat_data->message;

                const char *r_msg_time = format_time(chat_data->timestamp);

                GtkWidget *received_msg = gtk_label_new(r_msg);

                GtkWidget *received_time = gtk_label_new(r_msg_time);

                gtk_widget_set_halign(received_msg, GTK_ALIGN_START); // Выравнивание received_msg по левому краю

                gtk_widget_set_halign(received_time, GTK_ALIGN_END); // Выравнивание received_time по левому краю

                gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
                gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
                gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 30);
                gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

                gtk_widget_set_hexpand(received_msg, TRUE);

                GtkWidget *received_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
                gtk_box_append(GTK_BOX(received_box), received_msg);
                gtk_box_append(GTK_BOX(received_box), received_time);

                gtk_widget_set_margin_top(received_box, 15);

                gtk_widget_set_halign(received_box, GTK_ALIGN_START);
                gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_box, 0, 9999 + pos, 1, 1);

                widget_styling(received_msg, current_screen, "received_message");

                widget_styling(received_time, current_screen, "time");
            }

            chat_history_iter = chat_history_iter->next;

            count++;
        }
    }
    g_idle_add(scroll_to_bottom, chat_with_friend_scrolled);

    pthread_mutex_unlock(&mutex_send);
}

void update_current_chat(t_chat *chat_data, const char *friendname)
{
    if (strcmp(chat_data->sender, current_client.login) == 0)
    {
        const char *s_msg = g_strdup(chat_data->message);

        const char *s_msg_time = format_time(chat_data->timestamp);

        GtkWidget *sent_msg = gtk_button_new_with_label(s_msg);

        GtkWidget *sent_time = gtk_label_new(s_msg_time);

        GtkWidget *edit_btn = gtk_button_new();

        GtkWidget *delete_btn = gtk_button_new();

        GtkLabel *sent_msg_label = GTK_LABEL(gtk_button_get_child(GTK_BUTTON(sent_msg)));

        gtk_label_set_wrap(GTK_LABEL(sent_msg_label), TRUE);
        gtk_label_set_wrap_mode(GTK_LABEL(sent_msg_label), PANGO_WRAP_WORD_CHAR);
        gtk_label_set_max_width_chars(GTK_LABEL(sent_msg_label), 30);
        gtk_label_set_selectable(GTK_LABEL(sent_msg_label), FALSE);

        gtk_widget_set_hexpand(sent_msg, TRUE);

        gtk_widget_set_halign(sent_msg, GTK_ALIGN_END); // Выравнивание received_msg по левому краю

        gtk_widget_set_halign(sent_time, GTK_ALIGN_START); // Выравнивание received_time по левому краю

        GtkWidget *sent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_append(GTK_BOX(sent_box), sent_time);
        gtk_box_append(GTK_BOX(sent_box), sent_msg);
        // gtk_box_append(GTK_BOX(sent_box), edit_btn);
        // gtk_box_append(GTK_BOX(sent_box), delete_btn);

        gtk_widget_set_size_request(edit_btn, 16, 16);
        gtk_widget_set_size_request(delete_btn, 16, 16);

        gtk_widget_set_margin_top(sent_box, 15);

        gtk_widget_set_halign(sent_box, GTK_ALIGN_END);
        gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_box, 1, 9999 + chat_data->id, 1, 1);

        widget_styling(sent_msg, current_screen, "sent_message");
        widget_styling(sent_time, current_screen, "time");
        widget_styling(edit_btn, current_screen, "edit_btn");
        widget_styling(delete_btn, current_screen, "delete_btn");

        SentMessageData *sent_message_data_edit = malloc(sizeof(SentMessageData));
        sent_message_data_edit->chat_data = chat_data;
        sent_message_data_edit->sent_box = sent_box;
        sent_message_data_edit->friend_data = NULL;
        sent_message_data_edit->for_edit = TRUE;
        sent_message_data_edit->entry = entry;
        sent_message_data_edit->friendname = friendname;

        g_object_set_data(G_OBJECT(sent_box), "s_msg", (gpointer)s_msg);

        g_signal_connect(sent_msg, "clicked", G_CALLBACK(on_sent_msg_clicked), sent_message_data_edit);
    }
    else
    {
        const char *r_msg = chat_data->message;

        const char *r_msg_time = format_time(chat_data->timestamp);

        GtkWidget *received_msg = gtk_label_new(r_msg);

        GtkWidget *received_time = gtk_label_new(r_msg_time);

        gtk_widget_set_halign(received_msg, GTK_ALIGN_START); // Выравнивание received_msg по левому краю

        gtk_widget_set_halign(received_time, GTK_ALIGN_END); // Выравнивание received_time по левому краю

        gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
        gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
        gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 30);
        gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

        gtk_widget_set_hexpand(received_msg, TRUE);

        GtkWidget *received_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_append(GTK_BOX(received_box), received_msg);
        gtk_box_append(GTK_BOX(received_box), received_time);

        gtk_widget_set_margin_top(received_box, 15);

        gtk_widget_set_halign(received_box, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_box, 0, 9999 + chat_data->id, 1, 1);

        widget_styling(received_msg, current_screen, "received_message");

        widget_styling(received_time, current_screen, "time");
    }
    g_idle_add(scroll_to_bottom, chat_with_friend_scrolled);
}

void show_chat_with_friend(GtkWidget *btn, gpointer friend_data)
{
    (void)btn;
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), TRUE);

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.chat_with_friend);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.chat_with_friend))
    {
        gtk_widget_unparent(iter);
    }

    chat_with_friend_grid = create_grid(557, 513, "empty");

    GtkWidget *transparent_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(transparent_widget, FALSE);
    gtk_widget_set_vexpand(transparent_widget, TRUE);
    gtk_grid_attach(GTK_GRID(chat_with_friend_grid), transparent_widget, 0, 0, 1, 1);

    chat_with_friend_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), chat_with_friend_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_widget_set_hexpand(chat_with_friend_scrolled, FALSE);
    gtk_widget_set_hexpand(chat_with_friend_grid, FALSE);

    int last_child = 0;

    if (current_friend != NULL)
    {
        current_friend->in_chat = FALSE;
    }

    t_Friend *friend_iter = friend_data;

    show_friend_info(friend_data);
    friend_iter->in_chat = TRUE;
    current_friend = friend_iter;

    pthread_mutex_lock(&mutex_send);
    mx_printstr("show chat with : ");
    mx_printstr(friend_iter->username);
    mx_printstr("\n");

    t_list *chat_history_iter = friend_iter->chat_history;

    while (chat_history_iter != NULL)
    {
        t_chat *chat_data = (t_chat *)chat_history_iter->data;

        int pos = chat_data->id;

        if (strcmp(chat_data->sender, current_client.login) == 0)
        {
            const char *s_msg = g_strdup(chat_data->message);

            const char *s_msg_time = format_time(chat_data->timestamp);

            GtkWidget *sent_msg = gtk_button_new_with_label(s_msg);

            GtkWidget *sent_time = gtk_label_new(s_msg_time);

            GtkWidget *edit_btn = gtk_button_new();

            GtkWidget *delete_btn = gtk_button_new();

            GtkLabel *sent_msg_label = GTK_LABEL(gtk_button_get_child(GTK_BUTTON(sent_msg)));

            gtk_label_set_wrap(GTK_LABEL(sent_msg_label), TRUE);
            gtk_label_set_wrap_mode(GTK_LABEL(sent_msg_label), PANGO_WRAP_WORD_CHAR);
            gtk_label_set_max_width_chars(GTK_LABEL(sent_msg_label), 30);
            gtk_label_set_selectable(GTK_LABEL(sent_msg_label), FALSE);

            gtk_widget_set_hexpand(sent_msg, TRUE);

            gtk_widget_set_halign(sent_msg, GTK_ALIGN_END); // Выравнивание received_msg по левому краю

            gtk_widget_set_halign(sent_time, GTK_ALIGN_START); // Выравнивание received_time по левому краю

            GtkWidget *sent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            gtk_box_append(GTK_BOX(sent_box), sent_time);
            gtk_box_append(GTK_BOX(sent_box), sent_msg);
            // gtk_box_append(GTK_BOX(sent_box), edit_btn);
            // gtk_box_append(GTK_BOX(sent_box), delete_btn);

            gtk_widget_set_size_request(edit_btn, 16, 16);
            gtk_widget_set_size_request(delete_btn, 16, 16);

            gtk_widget_set_margin_top(sent_box, 15);

            gtk_widget_set_halign(sent_box, GTK_ALIGN_END);
            gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_box, 1, 9999 + pos, 1, 1);

            widget_styling(sent_msg, current_screen, "sent_message");
            widget_styling(sent_time, current_screen, "time");
            widget_styling(edit_btn, current_screen, "edit_btn");
            widget_styling(delete_btn, current_screen, "delete_btn");

            SentMessageData *sent_message_data_edit = malloc(sizeof(SentMessageData));
            sent_message_data_edit->chat_data = chat_data;
            sent_message_data_edit->sent_box = sent_box;
            sent_message_data_edit->friend_data = friend_iter;
            sent_message_data_edit->for_edit = TRUE;
            sent_message_data_edit->entry = entry;
            sent_message_data_edit->friendname = NULL;

            g_object_set_data(G_OBJECT(sent_box), "s_msg", (gpointer)s_msg);
             
            printf("s_msg: %s\n", s_msg);

            g_signal_connect(sent_msg, "clicked", G_CALLBACK(on_sent_msg_clicked), sent_message_data_edit);
            // free(sent_message_data_edit);
        }
        else
        {
            const char *r_msg = chat_data->message;

            const char *r_msg_time = format_time(chat_data->timestamp);

            GtkWidget *received_msg = gtk_label_new(r_msg);

            GtkWidget *received_time = gtk_label_new(r_msg_time);

            gtk_widget_set_halign(received_msg, GTK_ALIGN_START); // Выравнивание received_msg по левому краю

            gtk_widget_set_halign(received_time, GTK_ALIGN_END); // Выравнивание received_time по левому краю

            gtk_label_set_wrap(GTK_LABEL(received_msg), TRUE);
            gtk_label_set_wrap_mode(GTK_LABEL(received_msg), PANGO_WRAP_WORD_CHAR);
            gtk_label_set_max_width_chars(GTK_LABEL(received_msg), 30);
            gtk_label_set_selectable(GTK_LABEL(received_msg), FALSE);

            gtk_widget_set_hexpand(received_msg, TRUE);

            GtkWidget *received_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            gtk_box_append(GTK_BOX(received_box), received_msg);
            gtk_box_append(GTK_BOX(received_box), received_time);

            gtk_widget_set_margin_top(received_box, 15);

            gtk_widget_set_halign(received_box, GTK_ALIGN_START);
            gtk_grid_attach(GTK_GRID(chat_with_friend_grid), received_box, 0, 9999 + pos, 1, 1);

            widget_styling(received_msg, current_screen, "received_message");

            widget_styling(received_time, current_screen, "time");
        }
        chat_history_iter = chat_history_iter->next;

        last_child++;
    }

    // GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Write somethings...");

    // gtk_box_append(GTK_BOX(box), entry);

    gtk_widget_set_margin_start(entry, 17);
    gtk_widget_set_margin_end(entry, 17);
    gtk_widget_set_margin_top(entry, 14);
    gtk_widget_set_margin_bottom(entry, 14);

    // gtk_widget_set_margin_start(box, 26);
    // gtk_widget_set_margin_end(box, 60);
    // gtk_widget_set_margin_top(box, 10);
    // gtk_widget_set_margin_bottom(box, 14);

    gtk_widget_set_size_request(entry, 452, 40);

    gtk_widget_set_margin_top(chat_with_friend_scrolled, 75);
    gtk_widget_set_size_request(chat_with_friend_scrolled, 533, 513);

    gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), chat_with_friend_scrolled, 0, 0, 2, 9999 - last_child);
    gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), entry, 0, 9999 - last_child, 2, 1);

    // handler_id *handler_id = malloc(sizeof(handler_id));

    // handler_id->friend_data = friend_iter;
    // handler_id->id = 0;

    SentMessageData *sent_message_data_send = malloc(sizeof(SentMessageData));

    sent_message_data_send->chat_data = NULL;
    sent_message_data_send->friend_data = friend_iter;
    sent_message_data_send->entry = entry;
    sent_message_data_send->friendname = friend_iter->username;

    sent_message_data_send->for_edit = FALSE;

    // if (sent_message_data->id == 0)
    // {
    g_signal_handlers_disconnect_by_func(entry, (gpointer)G_CALLBACK(on_entry_activate_for_editing), sent_message_data_send);
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), sent_message_data_send);
    //}

    // widget_styling(box, current_screen, "entry_box");
    widget_styling(entry, current_screen, "entry_for_users_text");
    pthread_mutex_unlock(&mutex_send);
}

void update_current_chat_while_delete(t_Friend *friend_data, int old_message_id, const char *msg)
{
    t_list *deleted_message_data = friend_data->chat_history;
    t_chat *current_chat = NULL;

    while (deleted_message_data != NULL)
    {
        current_chat = (t_chat *)deleted_message_data->data;

        if (current_chat->id == old_message_id)
        {
            // Найдено сообщение с old_message_id
            break;
        }

        deleted_message_data = deleted_message_data->next;
    }

    GtkWidget *children, *sent_box, *btn;

    GtkWidget *children_temp, *btn_temp;

    children = gtk_widget_get_first_child(chat_with_friend_grid);

    GtkWidget *second_child = gtk_widget_get_next_sibling(children);

    for (sent_box = second_child; sent_box != NULL; sent_box = gtk_widget_get_next_sibling(sent_box))
    {
        btn = gtk_widget_get_last_child(sent_box); // sent_msg_btn

        const char *deleted_message_text = g_object_get_data(G_OBJECT(sent_box), "s_msg");


        if (strcmp(msg, deleted_message_text) == 0)
        {
            gtk_widget_unparent(sent_box);
            break;
        }
    }
}

void update_show_chats_with_added_friends(t_list *friend_list)
{
    pthread_mutex_lock(&mutex_send);

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

        GtkWidget *user_box_grid = create_grid(380, 80, NULL);

        GtkWidget *username_label = gtk_label_new(friend_data->username);

        get_scaled_image_chats_for_friend_list(friend_data);

        GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

        t_list *last_chat = friend_data->chat_history;

        while (last_chat != NULL && last_chat->next != NULL)
        {
            last_chat = last_chat->next;
        }

        GtkWidget *last_msg_label;

        if (last_chat != NULL && strcmp(((t_chat *)last_chat->data)->sender, current_client.login) == 0)
        {
            char *last_msg_you_sent = g_strconcat("You: ", friend_data->lastmessage, NULL);

            last_msg_label = gtk_label_new(last_msg_you_sent);

            g_free(last_msg_you_sent); // Освободите память, выделенную для строки last_msg_you_sent
        }
        else
        {
            last_msg_label = gtk_label_new(friend_data->lastmessage);
        }

        // gtk_label_set_width_chars(GTK_LABEL(last_msg_label), 30); // Установите максимальную ширину символов.
        gtk_label_set_max_width_chars(GTK_LABEL(last_msg_label), 30);
        gtk_label_set_ellipsize(GTK_LABEL(last_msg_label), PANGO_ELLIPSIZE_END); // Установите обрезку текста с многоточием.
        gtk_label_set_wrap(GTK_LABEL(last_msg_label), FALSE);

        gtk_grid_attach(GTK_GRID(user_box_grid), user_avatar, 0, 0, 1, 2);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), username_label);
        gtk_box_append(GTK_BOX(box), last_msg_label);

        gtk_grid_attach(GTK_GRID(user_box_grid), box, 1, 0, 1, 1);

        gtk_widget_set_margin_start(box, 8);

        gtk_widget_set_size_request(box, 180, 80);

        // gtk_grid_attach(GTK_GRID(user_box_grid), last_msg_label, 1, 1, 1, 1);

        gtk_button_set_child(GTK_BUTTON(user_box_btn1), user_box_grid);

        // gtk_widget_set_halign(user_box_btn1, GTK_ALIGN_CENTER);
        // gtk_widget_set_hexpand(user_box_btn1, TRUE);
        gtk_widget_set_vexpand(user_box_btn1, FALSE);
        gtk_widget_set_vexpand(user_box_grid, FALSE);

        gtk_widget_set_margin_start(user_box_btn1, 13);
        gtk_widget_set_margin_end(user_box_btn1, 14);
        gtk_widget_set_margin_top(user_box_btn1, 5);
        gtk_widget_set_margin_bottom(user_box_btn1, 10);

        gtk_widget_set_size_request(user_box_btn1, 380, 80);

        gtk_widget_set_margin_start(user_avatar, 6);
        gtk_widget_set_margin_top(user_avatar, 10);
        gtk_widget_set_margin_bottom(user_avatar, 10);

        gtk_widget_set_halign(username_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(username_label, 15);
        // gtk_widget_set_margin_start(username_label, 10);

        gtk_widget_set_halign(last_msg_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(last_msg_label, 10);
        // gtk_widget_set_margin_start(last_msg_label, 10);

        gtk_widget_set_size_request(user_avatar, 60, 60);

        gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn1, 0, count, 1, 1);
        count++;

        g_signal_connect(user_box_btn1, "clicked", G_CALLBACK(show_chat_with_friend), friend_data);

        widget_styling(user_box_btn1, current_screen, "user_box_btn");

        widget_styling(username_label, current_screen, "chat_gpt_message");

        widget_styling(last_msg_label, current_screen, "lastmessage");

        // widget_styling(box, current_screen, "chats_list_grid");

        current = current->next;
    }
    pthread_mutex_unlock(&mutex_send);
}

void update_show_friend_info(gpointer data)
{
    t_Friend *friend_data = data;

    GtkWidget *children;

    children = gtk_widget_get_first_child(current_grid.chat_with_friend);

    gtk_widget_unparent(children);

    GtkWidget *user_info_grid = create_grid(428, 75, NULL);

    get_scaled_image_chats_for_friend_list(friend_data);

    GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

    mx_printstr(friend_data->username);
    GtkWidget *username_label = gtk_label_new(friend_data->username);

    GtkWidget *is_online_label;

    if (!friend_data->connected)
    {

        is_online_label = gtk_label_new("Offline");
    }
    else
    {
        is_online_label = gtk_label_new("Online");
    }

    gtk_grid_attach(GTK_GRID(user_info_grid), user_avatar, 0, 0, 1, 2);

    GtkWidget *labels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(labels_box), username_label);
    gtk_box_append(GTK_BOX(labels_box), is_online_label);

    gtk_grid_attach(GTK_GRID(user_info_grid), labels_box, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), user_info_grid, 0, 0, 1, 1);

    gtk_widget_set_hexpand(current_grid.chat_with_friend, FALSE);

    gtk_widget_set_margin_start(user_avatar, 22);
    gtk_widget_set_margin_top(user_avatar, 10);
    gtk_widget_set_margin_bottom(user_avatar, 10);

    gtk_widget_set_size_request(user_avatar, 60, 60);

    gtk_widget_set_margin_start(labels_box, 13);
    gtk_widget_set_margin_top(labels_box, 15);

    gtk_widget_set_margin_top(is_online_label, 15);

    gtk_widget_set_halign(is_online_label, GTK_ALIGN_START);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    widget_styling(username_label, current_screen, "chat_gpt_message");

    widget_styling(is_online_label, current_screen, "is_online_label");

    // widget_styling(user_info_grid, current_screen, "chats_list_grid");

    // widget_styling(labels_box, current_screen, "chats_list_grid");
}

void show_friend_info(gpointer data)
{
    t_Friend *friend_data = data;

    GtkWidget *user_info_grid = create_grid(428, 75, NULL);

    get_scaled_image_chats_for_friend_list(friend_data);

    GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

    mx_printstr(friend_data->username);
    GtkWidget *username_label = gtk_label_new(friend_data->username);

    GtkWidget *is_online_label;

    if (!friend_data->connected)
    {

        is_online_label = gtk_label_new("Offline");
    }
    else
    {
        is_online_label = gtk_label_new("Online");
    }

    gtk_grid_attach(GTK_GRID(user_info_grid), user_avatar, 0, 0, 1, 2);

    GtkWidget *labels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(labels_box), username_label);
    gtk_box_append(GTK_BOX(labels_box), is_online_label);

    gtk_grid_attach(GTK_GRID(user_info_grid), labels_box, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), user_info_grid, 0, 0, 1, 1);

    gtk_widget_set_hexpand(current_grid.chat_with_friend, FALSE);

    gtk_widget_set_margin_start(user_avatar, 22);
    gtk_widget_set_margin_top(user_avatar, 10);
    gtk_widget_set_margin_bottom(user_avatar, 10);

    gtk_widget_set_size_request(user_avatar, 60, 60);

    gtk_widget_set_margin_start(labels_box, 13);
    gtk_widget_set_margin_top(labels_box, 15);

    gtk_widget_set_margin_top(is_online_label, 15);

    gtk_widget_set_halign(is_online_label, GTK_ALIGN_START);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    widget_styling(username_label, current_screen, "chat_gpt_message");

    widget_styling(is_online_label, current_screen, "is_online_label");

    // widget_styling(user_info_grid, current_screen, "chats_list_grid");

    // widget_styling(labels_box, current_screen, "chats_list_grid");
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

        GtkWidget *user_box_grid = create_grid(380, 80, NULL);

        GtkWidget *username_label = gtk_label_new(friend_data->username);

        get_scaled_image_chats_for_friend_list(friend_data);

        GtkWidget *user_avatar = gtk_image_new_from_pixbuf(scaled_avatar);

        t_list *last_chat = friend_data->chat_history;

        while (last_chat != NULL && last_chat->next != NULL)
        {
            last_chat = last_chat->next;
        }

        GtkWidget *last_msg_label;

        if (last_chat != NULL && strcmp(((t_chat *)last_chat->data)->sender, current_client.login) == 0)
        {
            char *last_msg_you_sent = g_strconcat("You: ", friend_data->lastmessage, NULL);

            last_msg_label = gtk_label_new(last_msg_you_sent);

            g_free(last_msg_you_sent); // Освободите память, выделенную для строки last_msg_you_sent
        }
        else
        {
            last_msg_label = gtk_label_new(friend_data->lastmessage);
        }

        // gtk_label_set_width_chars(GTK_LABEL(last_msg_label), 30); // Установите максимальную ширину символов.
        gtk_label_set_max_width_chars(GTK_LABEL(last_msg_label), 30);
        gtk_label_set_ellipsize(GTK_LABEL(last_msg_label), PANGO_ELLIPSIZE_END); // Установите обрезку текста с многоточием.
        gtk_label_set_wrap(GTK_LABEL(last_msg_label), FALSE);

        gtk_grid_attach(GTK_GRID(user_box_grid), user_avatar, 0, 0, 1, 2);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), username_label);
        gtk_box_append(GTK_BOX(box), last_msg_label);

        gtk_grid_attach(GTK_GRID(user_box_grid), box, 1, 0, 1, 1);

        gtk_widget_set_margin_start(box, 8);

        gtk_widget_set_size_request(box, 180, 80);

        // gtk_grid_attach(GTK_GRID(user_box_grid), last_msg_label, 1, 1, 1, 1);

        gtk_button_set_child(GTK_BUTTON(user_box_btn1), user_box_grid);

        // gtk_widget_set_halign(user_box_btn1, GTK_ALIGN_CENTER);
        // gtk_widget_set_hexpand(user_box_btn1, TRUE);
        gtk_widget_set_vexpand(user_box_btn1, FALSE);
        gtk_widget_set_vexpand(user_box_grid, FALSE);

        gtk_widget_set_margin_start(user_box_btn1, 13);
        gtk_widget_set_margin_end(user_box_btn1, 14);
        gtk_widget_set_margin_top(user_box_btn1, 5);
        gtk_widget_set_margin_bottom(user_box_btn1, 10);

        gtk_widget_set_size_request(user_box_btn1, 380, 80);

        gtk_widget_set_margin_start(user_avatar, 6);
        gtk_widget_set_margin_top(user_avatar, 10);
        gtk_widget_set_margin_bottom(user_avatar, 10);

        gtk_widget_set_halign(username_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(username_label, 15);
        // gtk_widget_set_margin_start(username_label, 10);

        gtk_widget_set_halign(last_msg_label, GTK_ALIGN_START);
        gtk_widget_set_margin_top(last_msg_label, 10);
        // gtk_widget_set_margin_start(last_msg_label, 10);

        gtk_widget_set_size_request(user_avatar, 60, 60);

        gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn1, 0, count, 1, 1);
        count++;

        g_signal_connect(user_box_btn1, "clicked", G_CALLBACK(show_chat_with_friend), friend_data);

        widget_styling(user_box_btn1, current_screen, "user_box_btn");

        widget_styling(username_label, current_screen, "chat_gpt_message");

        widget_styling(last_msg_label, current_screen, "lastmessage");

        // widget_styling(box, current_screen, "chats_list_grid");

        current = current->next;
    }
}

void show_create_new_chat_with_someone()
{

    chat_with_friend_grid = create_grid(557, 607, "empty");

    GtkWidget *transparent_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(transparent_widget, FALSE);
    gtk_widget_set_vexpand(transparent_widget, TRUE);
    gtk_grid_attach(GTK_GRID(chat_with_friend_grid), transparent_widget, 0, 0, 1, 1);

    chat_with_friend_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), chat_with_friend_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

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

    gtk_widget_set_margin_top(create_new_chat_with_someone_label, 83);

    gtk_widget_set_halign(create_new_chat_with_someone_label, GTK_ALIGN_CENTER);

    gtk_widget_set_hexpand(create_new_chat_with_someone_label, TRUE);

    gtk_widget_set_halign(user_list_grid_scrolled, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(user_list_grid_scrolled, GTK_ALIGN_CENTER);

    gtk_widget_set_hexpand(user_list_grid_scrolled, TRUE);
    gtk_widget_set_vexpand(user_list_grid_scrolled, TRUE);

    gtk_widget_set_size_request(user_list_grid_scrolled, 451, 227);

    gtk_widget_set_halign(user_list_grid_scrolled, GTK_ALIGN_CENTER);

    // gtk_widget_set_margin_start(entry_for_search, 128);
    // gtk_widget_set_margin_end(entry_for_search, 129);
    gtk_widget_set_margin_top(entry_for_search, 47);

    gtk_widget_set_halign(entry_for_search, GTK_ALIGN_CENTER);

    gtk_widget_set_hexpand(entry_for_search, TRUE);

    gtk_widget_set_size_request(entry_for_search, 300, 45);

    widget_styling(create_new_chat_with_someone_label, current_screen, "create_new_chat_with_someone_label");
    widget_styling(entry_for_search, current_screen, "entry_for_search_user");

    g_signal_connect(entry_for_search, "changed", G_CALLBACK(update_user_list_while_searching), NULL);
}
