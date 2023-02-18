#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void show_chats() {
    GtkWidget *chats_container_lab = gtk_label_new("chats");

    gtk_grid_attach(GTK_GRID(curent_grid.chats), chats_container_lab, 0, 0, 1, 1);
}

void show_search_bar() {
    GtkWidget *search_lab = gtk_label_new("SEARCH BAR");

    gtk_grid_attach(GTK_GRID(curent_grid.search_bar), search_lab, 0, 0, 1, 1);
}

void show_mini_chats() {
    GtkWidget *chats_label = gtk_label_new("Chats");

    gtk_grid_attach(GTK_GRID(curent_grid.mini_chats), chats_label, 0, 0, 1, 1);
}

void show_mini_groups() {
    GtkWidget *groups_label = gtk_label_new("Groups");

    gtk_grid_attach(GTK_GRID(curent_grid.mini_groups), groups_label, 0, 0, 1, 1);
}
