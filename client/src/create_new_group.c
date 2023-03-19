#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

static void show_chats_clicked() {
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), TRUE);
}

void create_new_group(const int i,
                     const char *new_groupname) {

//    GtkWidget *new_chat = create_grid(387, 50, "chat_line");

    GtkWidget *new_group = gtk_button_new();
    GtkWidget *avatar_grid = create_grid(50, 50, "avatar_grid");
    GtkWidget *avatar = gtk_image_new_from_file("resources/images/test_avatar1.jpg");

    //grid for name and last message
    GtkWidget *grid = create_grid(337, 50, NULL);
    GtkWidget *groupname = gtk_label_new(new_groupname);
    GtkWidget *message = gtk_label_new("Test Message");


    gtk_widget_set_size_request(new_group, 387, 50);
    gtk_widget_set_size_request(avatar, 50, 50);
    gtk_widget_set_size_request(groupname, 337, 25);
    gtk_widget_set_size_request(message, 337, 25);

    widget_styling(new_group, current_screen, "chat_line");
    widget_styling(avatar_grid, current_screen, "avatar_grid");
    widget_styling(groupname, current_screen, "chat_username");
    widget_styling(message, current_screen, "chat_message");


//    gtk_grid_attach(GTK_GRID(new_chat), avatar_grid, 0, 0, 1, 1);
    gtk_button_set_child(GTK_BUTTON(new_group), avatar_grid);
    gtk_grid_attach(GTK_GRID(avatar_grid), avatar, 0, 0, 1, 1);

    gtk_button_set_child(GTK_BUTTON(new_group), grid);
//    gtk_grid_attach(GTK_GRID(new_chat), grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), groupname, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.goups_list_grid_child), new_group, 0, i, 1, 1);

    g_signal_connect(new_group, "clicked", G_CALLBACK(show_chats_clicked), NULL);
}
