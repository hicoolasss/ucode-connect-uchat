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

void show_mini_chats() {
    GtkWidget *chats_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *chats_label = gtk_label_new("Chats");
    GtkWidget *create_new_chat_btn = gtk_button_new();

    gtk_widget_set_size_request(chats_box, 387, 30);
    gtk_widget_set_size_request(chats_label, 61, 30);
    gtk_widget_set_size_request(create_new_chat_btn, 21, 21);

    gtk_widget_set_margin_start(create_new_chat_btn, 305);

    widget_styling(chats_box, curent_screen, "mini_groups_box");
    widget_styling(chats_label, curent_screen, "group_label");
    widget_styling(create_new_chat_btn, curent_screen, "create_new_chat_btn");

    gtk_box_append(GTK_BOX(chats_box), chats_label);
    gtk_box_append(GTK_BOX(chats_box), create_new_chat_btn);

    gtk_grid_attach(GTK_GRID(curent_grid.mini_chats), chats_box, 0, 0, 1, 1);
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
