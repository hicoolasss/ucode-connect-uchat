#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;


void show_chats() {
    GtkWidget *chats_container_lab = gtk_label_new("chats");

    gtk_grid_attach(GTK_GRID(curent_grid.chats), chats_container_lab, 0, 0, 1, 1);
}

void show_search_bar() {

    GtkWidget *search_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *search_btn = gtk_button_new();
    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), "Search");

    gtk_widget_set_size_request(search_bar_box, 391, 24);
    gtk_widget_set_size_request(entry_for_search, 357, 21);

    widget_styling(search_bar_box, curent_screen, "search_bar_box");
    widget_styling(search_btn, curent_screen, "search_btn");
    widget_styling(entry_for_search, curent_screen, "entry_for_search");

    gtk_box_append(GTK_BOX(search_bar_box), search_btn);
    gtk_box_append(GTK_BOX(search_bar_box), entry_for_search);

    gtk_grid_attach(GTK_GRID(curent_grid.search_bar), search_bar_box, 0, 0, 1, 1);
}

void create_new_chat(const int i) {
    GtkWidget *new_chat = create_grid(387, 50, "chat_line");
    GtkWidget *avatar_grid = create_grid(50, 50, "avatar_grid");
    GtkWidget *avatar = gtk_image_new_from_file("resources/images/test_avatar1.jpg");

    //grid for name and last message
    GtkWidget *grid = create_grid(337, 50, NULL);
    GtkWidget *username = gtk_label_new("Test User");
    GtkWidget *message = gtk_label_new("Test Message");


    gtk_widget_set_size_request(avatar, 50, 50);
    gtk_widget_set_size_request(username, 337, 25);
    gtk_widget_set_size_request(message, 337, 25);

    widget_styling(avatar_grid, curent_screen, "avatar_grid");
    widget_styling(username, curent_screen, "chat_username");
    widget_styling(message, curent_screen, "chat_message");


    gtk_grid_attach(GTK_GRID(new_chat), avatar_grid, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(avatar_grid), avatar, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(new_chat), grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.chats_list_grid_child), new_chat, 0, i, 1, 1);

}

static void create_new_chat_clicked() {
    create_new_chat(curent_grid.chat_pos_count);

    curent_grid.chat_pos_count++;
}

void show_mini_chats() {
    GtkWidget *chats_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *chats_label = gtk_label_new("Chats");
    GtkWidget *create_new_chat_btn = gtk_button_new();
    curent_grid.chat_pos_count = 0;


    curent_grid.chats_list_grid_child = create_grid(387, 166, "chats_list_grid_child");

    GtkWidget *chats_list_grid = create_grid(387, 166, "chats_list_grid");


    //scroll
    GtkWidget *chats_list_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chats_list_grid_scrolled), curent_grid.chats_list_grid_child);
//    gtk_scrolled_window_set_propagate_natural_height (GTK_SCROLLED_WINDOW(chats_list_grid_scrolled),false);
//    gtk_scrolled_window_set_propagate_natural_width (GTK_SCROLLED_WINDOW(chats_list_grid_scrolled),true);

    gtk_widget_set_size_request(chats_list_grid_scrolled, 400, 166);

    gtk_widget_set_size_request(chats_box, 387, 30);
    gtk_widget_set_size_request(chats_label, 61, 30);
    gtk_widget_set_size_request(create_new_chat_btn, 21, 21);

    gtk_widget_set_margin_start(create_new_chat_btn, 305);

    widget_styling(chats_box, curent_screen, "mini_groups_box");
    widget_styling(chats_label, curent_screen, "group_label");
    widget_styling(create_new_chat_btn, curent_screen, "create_new_chat_btn");
    widget_styling(chats_list_grid_scrolled, curent_screen, "chats_list_grid_scrolled");

    gtk_box_append(GTK_BOX(chats_box), chats_label);
    gtk_box_append(GTK_BOX(chats_box), create_new_chat_btn);

    //create new chat on + click
    g_signal_connect(create_new_chat_btn, "clicked", G_CALLBACK(create_new_chat_clicked), NULL);

    gtk_grid_attach(GTK_GRID(curent_grid.mini_chats), chats_box, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.mini_chats), chats_list_grid, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(chats_list_grid), chats_list_grid_scrolled, 0, 0, 1, 1);
}

void show_mini_groups() {
    GtkWidget *groups_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *groups_label = gtk_label_new("Groups");
    GtkWidget *create_new_group_btn = gtk_button_new();

    gtk_widget_set_size_request(groups_label, 387, 30);
    gtk_widget_set_size_request(groups_label, 74, 30);
    gtk_widget_set_size_request(create_new_group_btn, 21, 21);

    gtk_widget_set_margin_start(create_new_group_btn, 292);

    widget_styling(groups_box, curent_screen, "mini_groups_box");
    widget_styling(groups_label, curent_screen, "group_label");
    widget_styling(create_new_group_btn, curent_screen, "create_new_chat_btn");

    gtk_box_append(GTK_BOX(groups_box), groups_label);
    gtk_box_append(GTK_BOX(groups_box), create_new_group_btn);

    gtk_grid_attach(GTK_GRID(curent_grid.mini_groups), groups_box, 0, 0, 1, 1);
}
