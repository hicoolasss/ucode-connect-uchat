#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

void call_new_group_and_add_iter(const gchar *const new_username){
    create_new_group(current_grid.group_pos_count,
                    new_username);

    current_grid.group_pos_count++;
}

static void create_new_group_clicked() {
//    current_grid.group_search = TRUE;
    gtk_entry_set_activates_default(GTK_ENTRY(current_grid.entry_for_search), TRUE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_grid.entry_for_search), "New group name");
    g_signal_connect(current_grid.search_btn, "clicked", G_CALLBACK(search_btn_clicked_group), NULL);
    search_btn_clicked_group();
//    call_new_group_and_add_iter();
}

void show_mini_groups() {
    GtkWidget *groups_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *groups_label = gtk_label_new("Groups");
    GtkWidget *create_new_group_btn = gtk_button_new();
    current_grid.group_pos_count = 0;

    current_grid.goups_list_grid_child = create_grid(387, 166, "chats_list_grid_child");

    GtkWidget *groups_list_grid = create_grid(387, 166, "chats_list_grid");

    //scroll
    GtkWidget *groups_list_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(groups_list_grid_scrolled), current_grid.goups_list_grid_child);

    gtk_widget_set_size_request(groups_list_grid_scrolled, 400, 166);

    gtk_widget_set_size_request(groups_box, 387, 30);
//    gtk_widget_set_size_request(groups_label, 387, 30);
    gtk_widget_set_size_request(groups_label, 74, 30);
    gtk_widget_set_size_request(create_new_group_btn, 21, 21);

    gtk_widget_set_margin_start(create_new_group_btn, 292);

    widget_styling(groups_box, current_screen, "mini_groups_box");
    widget_styling(groups_label, current_screen, "group_label");
    widget_styling(create_new_group_btn, current_screen, "create_new_chat_btn");

    gtk_box_append(GTK_BOX(groups_box), groups_label);
    gtk_box_append(GTK_BOX(groups_box), create_new_group_btn);

    //create new chat on + click
    g_signal_connect(create_new_group_btn, "clicked", G_CALLBACK(create_new_group_clicked), NULL);

    gtk_grid_attach(GTK_GRID(current_grid.mini_groups), groups_box, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.mini_groups), groups_list_grid, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(groups_list_grid), groups_list_grid_scrolled, 0, 0, 1, 1);
}
