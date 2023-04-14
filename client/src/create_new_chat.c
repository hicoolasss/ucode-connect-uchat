#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;
extern int in_chat;

GdkPixbuf *scaled_avatar;

GtkWidget *user_list_grid;
GtkWidget *user_list_grid_scrolled;
GtkWidget *checkbox_btn;

int count = 0;
int temp_count = 0;

static void get_scaled_image_chats()
{

    // GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(current_avatar.avatar, NULL);
    if (!current_avatar.avatar)
    {
        g_print("Ошибка при загрузке изображения.\n");
        return;
    }

    // Масштабирование исходного изображения до размера аватара
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(current_avatar.avatar, 55, 55, GDK_INTERP_BILINEAR);
    // g_object_unref(current_avatar.avatar);

    // Создание поверхности Cairo для рисования
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 55, 55);
    cairo_t *cr = cairo_create(surface);

    // Создание круглой области
    cairo_arc(cr, 55 / 2.0, 55 / 2.0, 55 / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    GdkPixbuf *circle_pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 55, 55);

    scaled_avatar = circle_pixbuf;
}
void create_new_chat(GtkToggleButton *toggle_button, gpointer user_data)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<add_friend>");
    char *friend = mx_strdup(((t_user *)user_data)->username);
    cJSON_AddStringToObject(json, "friend", friend);
    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    GtkWidget *user_info_box_temp = gtk_widget_get_parent(GTK_WIDGET(toggle_button));

    GtkWidget *checkbox_btn = gtk_widget_get_last_child(user_info_box_temp);

    // Отсоедините checkbox_btn от user_info_box
    gtk_box_remove(GTK_BOX(user_info_box_temp), checkbox_btn);

    // Отсоедините user_info_box от его текущего родителя
    gtk_widget_unparent(user_info_box_temp);

    GtkWidget *user_box_btn = gtk_button_new();

    gtk_widget_set_margin_start(user_info_box_temp, 0);
    gtk_widget_set_margin_end(user_info_box_temp, 0);
    gtk_widget_set_margin_top(user_info_box_temp, 0);
    gtk_widget_set_margin_bottom(user_info_box_temp, 0);

    gtk_button_set_child(GTK_BUTTON(user_box_btn), user_info_box_temp);

    gtk_widget_set_size_request(user_box_btn, 400, 55);

    gtk_widget_set_margin_top(user_box_btn, 5);
    gtk_widget_set_margin_bottom(user_box_btn, 10);

    gtk_widget_set_size_request(current_grid.mini_chats, 427, 246);

    gtk_widget_set_size_request(current_grid.chats_list_grid_child, 427, 246);

    gtk_widget_set_hexpand(current_grid.chats_list_grid_child, FALSE);

    g_print("current_grid.chats_list_grid_child: %p\n", (void *)current_grid.chats_list_grid_child);

    gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn, 0, count, 1, 1);
    count++;

    GtkStyleContext *context = gtk_widget_get_style_context(user_info_box_temp);

    gtk_style_context_remove_class(context, "user_info_box");

    widget_styling(user_box_btn, current_screen, "user_box_btn");
}

static void show_user_list_scrolled(t_list *current)
{

    int pos = 0;

    get_scaled_image_chats();
    // mx_printstr(((t_user*)user_list->data)->username);
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

        gtk_widget_set_size_request(user_info_box, 400, 55);

        gtk_widget_set_halign(user_info_box, GTK_ALIGN_CENTER);

        gtk_widget_set_margin_start(user_avatar, 0);
        gtk_widget_set_margin_top(user_avatar, 0);
        gtk_widget_set_margin_bottom(user_avatar, 0);

        gtk_widget_set_size_request(user_avatar, 55, 55);

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

static void on_entry_activate(GtkEntry *entry, gpointer data)
{
    GtkEntryBuffer *text = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char *username = (const char *)data;
    const char *message = g_strdup(gtk_entry_buffer_get_text(text));
    // Теперь переменная text_copy содержит текст из виджета GtkEntry
    // Вы можете использовать text_copy для дальнейших действий, например, отправки сообщения
    // printf("Username: %s\n", username);
    // printf("Entry text: %s\n", message);
    printf("%s -> %s\n", username, message);
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<send_message_to>");
    cJSON_AddStringToObject(json, "friend", username);
    cJSON_AddStringToObject(json, "message", message);
    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    pthread_mutex_lock(&mutex1);
    in_chat = 1;
    pthread_cond_signal(&new_data_cond);
    pthread_mutex_unlock(&mutex1);
    // Не забудьте освободить память, выделенную для text_copy, когда она вам больше не понадобится
}

void show_chat_with_friend(GtkWidget *btn, gpointer username_copy)
{

    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.empty_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), TRUE);

    in_chat = 1;

    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.chat_with_friend);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.chat_with_friend))
    {

        gtk_widget_unparent(iter);
    }

    GtkWidget *chat_with_friend_grid = create_grid(557, 507, "empty");

    GtkWidget *chat_with_friend_scrolled = gtk_scrolled_window_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), chat_with_friend_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_with_friend_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    t_list *chat_history = NULL;
    const int temp_size = 4096;
    char temp[temp_size];
    int last_child = 0;

    GtkWidget *child_username = gtk_widget_get_last_child(btn);

    const gchar *g_text = gtk_label_get_text(GTK_LABEL(child_username));

    const char *text = (const char *)g_text;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "<show_history>");
    cJSON_AddStringToObject(json, "friend", text);
    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    while (!chat_history)
    {
        pthread_mutex_lock(&mutex1);
        int bytes_received = SSL_read(current_client.ssl, temp, temp_size - 1);
        pthread_mutex_unlock(&mutex1);
        if (bytes_received <= 0)
        {
            break;
        }

        temp[bytes_received] = '\0';

        if (mx_strcmp(temp, "chat empty") == 0)
        {
            break;
        }

        chat_history = deserialize_chathistory_list(temp);
    }

    t_list *current = chat_history;
    if (chat_history)
    {

        while (current)
        {

            const char *s_msg = ((t_chat *)current->data)->message;

            GtkWidget *sent_msg = gtk_label_new(s_msg);

            // printf("aaa: %s\n", s_msg);

            gtk_widget_set_halign(sent_msg, GTK_ALIGN_END);

            gtk_widget_set_margin_top(sent_msg, 15);

            gtk_label_set_wrap(GTK_LABEL(sent_msg), TRUE);
            gtk_label_set_wrap_mode(GTK_LABEL(sent_msg), PANGO_WRAP_WORD_CHAR);
            gtk_label_set_max_width_chars(GTK_LABEL(sent_msg), 50);
            gtk_label_set_selectable(GTK_LABEL(sent_msg), FALSE);

            gtk_widget_set_hexpand(sent_msg, TRUE);
            // gtk_widget_set_size_request(sent_msg, 50, 40);

            int pos = ((t_chat *)current->data)->id;

            gtk_grid_attach(GTK_GRID(chat_with_friend_grid), sent_msg, 0, pos, 1, 1);

            current = current->next;

            last_child++;

            widget_styling(sent_msg, current_screen, "message");
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
        gtk_widget_set_margin_end(box, 55);
        gtk_widget_set_margin_top(box, 10);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_widget_set_margin_top(chat_with_friend_scrolled, 75);
        // gtk_widget_set_margin_bottom(chat_with_friend_scrolled, 16);
        gtk_widget_set_size_request(chat_with_friend_scrolled, 533, 507);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), chat_with_friend_scrolled, 0, 0, 1, 1);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 9999, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), username_copy);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");
    }
    else
    {
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        GtkWidget *entry = gtk_entry_new();

        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

        gtk_box_append(GTK_BOX(box), entry);

        gtk_widget_set_margin_start(entry, 17);
        gtk_widget_set_margin_end(entry, 17);
        gtk_widget_set_margin_top(entry, 14);
        gtk_widget_set_margin_bottom(entry, 14);

        gtk_widget_set_margin_start(box, 26);
        gtk_widget_set_margin_end(box, 55);
        gtk_widget_set_margin_top(box, 613);
        gtk_widget_set_margin_bottom(box, 14);

        gtk_widget_set_size_request(box, 452, 40);

        gtk_grid_attach(GTK_GRID(current_grid.chat_with_friend), box, 0, 0, 1, 1);

        g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), username_copy);

        widget_styling(box, current_screen, "empty_chat_box");
        widget_styling(entry, current_screen, "empty_chat_label");
    }

    while (chat_history != NULL)
    {
        t_list *tmp = chat_history;
        chat_history = chat_history->next;
        free(tmp->data);
        free(tmp);
    }
}

void show_chats_with_added_friends(const char *username)
{
    GtkWidget *user_box_btn1 = gtk_button_new();

    GtkWidget *username1 = gtk_label_new(username);

    gtk_button_set_child(GTK_BUTTON(user_box_btn1), username1);

    gtk_widget_set_halign(user_box_btn1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(user_box_btn1, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(user_box_btn1, TRUE);
    gtk_widget_set_vexpand(user_box_btn1, TRUE);

    gtk_widget_set_size_request(user_box_btn1, 400, 55);

    gtk_widget_set_margin_top(user_box_btn1, 5);
    gtk_widget_set_margin_bottom(user_box_btn1, 10);

    gtk_widget_set_size_request(current_grid.mini_chats, 427, 246);

    gtk_widget_set_size_request(current_grid.chats_list_grid_child, 427, 246);

    gtk_widget_set_hexpand(current_grid.chats_list_grid_child, FALSE);

    gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn1, 0, count, 1, 1);
    count++;

    gpointer username_copy = (gpointer)username;

    // g_print("%s\n", (const char *)username_copy);

    g_signal_connect(user_box_btn1, "clicked", G_CALLBACK(show_chat_with_friend), username_copy);

    widget_styling(user_box_btn1, current_screen, "user_box_btn");

    // g_free(username_copy);
}

void show_create_new_chat_with_someone()
{
    // user_list = user_list_temp;

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

    // g_print("penis[ig]: %p\n", (void *)current_grid.chats_list_grid_child);

    t_list *cur = user_list;

    show_user_list_scrolled(cur);

    // g_print("georjngjiroegnero[ig]: %p\n", (void *)current_grid.chats_list_grid_child);
    t_list *current = friend_list;

    while (current)
    {
        show_chats_with_added_friends(((t_user *)current->data)->username);
        current = current->next;
    }
}
