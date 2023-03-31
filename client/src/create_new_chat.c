#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

static void show_chats_clicked() {
    //gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), TRUE);
}

void create_new_chat(const int i,
                     const char *new_username) {

//    GtkWidget *new_chat = create_grid(387, 50, "chat_line");

    GtkWidget *new_chat = gtk_button_new();
    GtkWidget *avatar_grid = create_grid(50, 50, "avatar_grid");
    GtkWidget *avatar = gtk_image_new_from_file("/home/criops/ucode-connect-uchat/avatar1.png");

    //grid for name and last message
    GtkWidget *grid = create_grid(337, 50, NULL);
    GtkWidget *username = gtk_label_new(new_username);
    GtkWidget *message = gtk_label_new("Test Message");


    gtk_widget_set_size_request(new_chat, 387, 50);
    gtk_widget_set_size_request(avatar, 50, 50);
    gtk_widget_set_size_request(username, 337, 25);
    gtk_widget_set_size_request(message, 337, 25);

    widget_styling(new_chat, current_screen, "chat_line");
    widget_styling(avatar_grid, current_screen, "avatar_grid");
    widget_styling(username, current_screen, "chat_username");
    widget_styling(message, current_screen, "chat_message");


//    gtk_grid_attach(GTK_GRID(new_chat), avatar_grid, 0, 0, 1, 1);
    gtk_button_set_child(GTK_BUTTON(new_chat), avatar_grid);
    gtk_grid_attach(GTK_GRID(avatar_grid), avatar, 0, 0, 1, 1);

    gtk_button_set_child(GTK_BUTTON(new_chat), grid);
//    gtk_grid_attach(GTK_GRID(new_chat), grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), new_chat, 0, i, 1, 1);

    g_signal_connect(new_chat, "clicked", G_CALLBACK(show_chats_clicked), NULL);
}


void show_create_new_chat_with_someone() {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<user_list>");

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    t_list *user_list = receive_list(current_client.ssl);
    
    GtkWidget *create_new_chat_with_someone_label = gtk_label_new("Create new chat...");

    //GtkWidget *search_user = gtk_search_bar_new();

    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), "Add new chat");

    //gtk_search_bar_connect_entry(GTK_SEARCH_BAR(search_user), GTK_EDITABLE(entry_for_search));



    gtk_grid_attach(GTK_GRID(current_grid.chats), create_new_chat_with_someone_label, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.chats), entry_for_search, 0, 1, 1, 1);


    gtk_widget_set_margin_start(create_new_chat_with_someone_label, 16);
    gtk_widget_set_margin_end(create_new_chat_with_someone_label, 316);
    gtk_widget_set_margin_top(create_new_chat_with_someone_label, 9);

    gtk_widget_set_size_request(create_new_chat_with_someone_label, 225, 33);

    gtk_widget_set_margin_start(entry_for_search, 164);
    gtk_widget_set_margin_end(entry_for_search, 163);
    gtk_widget_set_margin_top(entry_for_search, 300);

    gtk_widget_set_size_request(entry_for_search, 230, 45);


    widget_styling(create_new_chat_with_someone_label, current_screen, "create_new_chat_with_someone_label");
    widget_styling(entry_for_search, current_screen, "chat_gpt_entry_box");

    //gtk_search_bar_set_key_capture_widget (GTK_SEARCH_BAR (search_user), current_screen.screen);

    //widget_styling(entry_for_search, current_screen, "auth_main_box");


}

