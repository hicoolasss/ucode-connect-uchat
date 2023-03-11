#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

void create_new_chat(const int i,
                     const char *new_username) {
    GtkWidget *new_chat = create_grid(387, 50, "chat_line");
    GtkWidget *avatar_grid = create_grid(50, 50, "avatar_grid");
    GtkWidget *avatar = gtk_image_new_from_file("resources/images/test_avatar1.jpg");

    //grid for name and last message
    GtkWidget *grid = create_grid(337, 50, NULL);
    GtkWidget *username = gtk_label_new(new_username);
    GtkWidget *message = gtk_label_new("Test Message");


    gtk_widget_set_size_request(avatar, 50, 50);
    gtk_widget_set_size_request(username, 337, 25);
    gtk_widget_set_size_request(message, 337, 25);

    widget_styling(avatar_grid, current_screen, "avatar_grid");
    widget_styling(username, current_screen, "chat_username");
    widget_styling(message, current_screen, "chat_message");


    gtk_grid_attach(GTK_GRID(new_chat), avatar_grid, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(avatar_grid), avatar, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(new_chat), grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.chats_list_grid_child), new_chat, 0, i, 1, 1);

}
