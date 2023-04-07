#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
GdkPixbuf *scaled_avatar;

GtkWidget *user_list_grid;
GtkWidget *user_list_grid_scrolled;
GtkWidget *checkbox_btn;

int count = 0;
int temp_count = 0;

t_list *user_list;
t_list *friend_list;

static void show_chats_clicked()
{
    // gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), TRUE);
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

static void on_entry_activate(GtkEntry *entry, gpointer data) {
    GtkEntryBuffer *text = gtk_entry_get_buffer(GTK_ENTRY(entry));
    const char *text_copy = mx_strdup(gtk_entry_buffer_get_text(text));
    // Теперь переменная text_copy содержит текст из виджета GtkEntry
    // Вы можете использовать text_copy для дальнейших действий, например, отправки сообщения
    printf("Entry text: %s\n", text_copy);

    // Не забудьте освободить память, выделенную для text_copy, когда она вам больше не понадобится

}

void show_chat_with_friend(gpointer username) {

    gchar *data = (gchar *)username;

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *entry = gtk_entry_new();

    GtkWidget *label = gtk_label_new(data);

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ti pidor");

    gtk_box_append(GTK_BOX(box), entry);
    //gtk_box_append(GTK_BOX(box), label);

    gtk_widget_set_margin_start(entry, 17);
    gtk_widget_set_margin_end(entry, 17);
    gtk_widget_set_margin_top(entry, 14);
    gtk_widget_set_margin_bottom(entry, 14);

    gtk_widget_set_margin_start(label, 17);
    gtk_widget_set_margin_end(label, 17);
    gtk_widget_set_margin_top(label, 14);
    //gtk_widget_set_margin_bottom(label, 14);

    gtk_widget_set_margin_start(box, 179);
    gtk_widget_set_margin_end(box, 178);
    gtk_widget_set_margin_top(box, 317);
    gtk_widget_set_margin_bottom(box, 316);

    gtk_widget_set_size_request(box, 200, 36);

    gtk_grid_attach(GTK_GRID(current_grid.empty_chat), box, 0, 0, 1, 1);

    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), NULL);

    widget_styling(box, current_screen, "empty_chat_box");
    widget_styling(entry, current_screen, "empty_chat_label");
    
}


static void show_user_list_scrolled()
{

    int pos = 0;

    t_list *current = user_list;
    get_scaled_image_chats();

    while (current != NULL)
    {
        pos++;

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

void create_new_chat(GtkToggleButton *toggle_button, gpointer user_data)
{
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<add_friend>");

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    int command = SSL_write(current_client.ssl, ((t_user *)user_data)->username, mx_strlen(((t_user *)user_data)->username));

    if (command < 0)
    {
        return;
    }
    else
    {
        mx_printstr(((t_user *)user_data)->username);
        mx_printchar('\n');
    }

    char temp[256];
    int rec = SSL_read(current_client.ssl, temp, sizeof(temp) - 1);
    if (rec == -1)
    {
        printf("Error receiving message\n");
    }

    if (mx_strcmp(temp, "User not found, write friend's login correct") == 0)
    {
        mx_printstr("User not found, write friend's login correct");
        return;
    }
    else
    {
        mx_printstr(temp);
        mx_printstr("\n\n");
    }

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

void show_chats_with_added_friends(const char *username1) {

    GtkWidget *user_box_btn1 = gtk_button_new();

    GtkWidget *username = gtk_label_new(username1);

    gtk_button_set_child(GTK_BUTTON(user_box_btn1), username);

    gtk_widget_set_size_request(user_box_btn1, 400, 55);

    gtk_widget_set_margin_top(user_box_btn1, 5);
    gtk_widget_set_margin_bottom(user_box_btn1, 10);

    gtk_widget_set_size_request(current_grid.mini_chats, 427, 246);

    gtk_widget_set_size_request(current_grid.chats_list_grid_child, 427, 246);


    gtk_widget_set_hexpand(current_grid.chats_list_grid_child, FALSE);

    gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), user_box_btn1, 0, count, 1, 1);
    count++;

    gchar *username_copy = g_strdup((const char *)username);
    
    g_signal_connect(user_box_btn1, "clicked", G_CALLBACK(show_chat_with_friend), username_copy);

    widget_styling(user_box_btn1, current_screen, "user_box_btn");

}

void show_create_new_chat_with_someone()
{

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<user_list>");

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    user_list = receive_list(current_client.ssl);


    // cJSON *json1 = cJSON_CreateObject();
    // cJSON_AddStringToObject(json1, "login", current_client.login);
    // cJSON_AddStringToObject(json1, "command", "<friend_list>");

    // char *json_str1 = cJSON_Print(json1);
    // cJSON_Delete(json1);

    // send_message_to_server(json_str1);

    // friend_list = receive_list(current_client.ssl);

    user_list_grid = create_grid(451, 227, "mini_chats");

    user_list_grid_scrolled = gtk_scrolled_window_new();
    
    current_grid.chat_with_friend = create_grid(557, 667, "mini_chats");

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

    //g_print("penis[ig]: %p\n", (void *)current_grid.chats_list_grid_child);
    
    show_user_list_scrolled();

    //g_print("georjngjiroegnero[ig]: %p\n", (void *)current_grid.chats_list_grid_child);
    t_list *current = friend_list;
    
    while (current) {
        show_chats_with_added_friends(((t_user *)current->data)->username);
        mx_printstr(((t_user *)current->data)->username);
        mx_printstr("\n");
        current = current->next;
    }

}