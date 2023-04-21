#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;


void call_new_chat_and_add_iter()
{
    // create_new_chat(current_grid.chat_pos_count,
    //                 new_username);

    current_grid.chat_pos_count++;
}

static void create_new_chat_clicked()
{
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.empty_chat), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), FALSE);

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<user_list>");
    g_async_queue_push(message_queue, json);
}

void show_mini_chats()
{
    GtkWidget *chats_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *chats_label = gtk_label_new("Chats");
    GtkWidget *create_new_chat_btn = gtk_button_new();
    current_grid.chat_pos_count = 0;

    current_grid.chats_list_grid_child = create_grid(400, 190, "chats_list_grid_child");

    // GtkWidget *chats_list_grid = create_grid(400, 190, "chats_list_grid");

    // scroll
    GtkWidget *chats_list_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chats_list_grid_scrolled), current_grid.chats_list_grid_child);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chats_list_grid_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_widget_set_size_request(chats_list_grid_scrolled, 400, 190);

    gtk_widget_set_margin_start(chats_list_grid_scrolled, 0);
    gtk_widget_set_margin_end(chats_list_grid_scrolled, 0);
    gtk_widget_set_margin_top(chats_list_grid_scrolled, 0);
    gtk_widget_set_margin_bottom(chats_list_grid_scrolled, 0);

    gtk_widget_set_size_request(chats_box, 387, 30);
    gtk_widget_set_size_request(chats_label, 61, 30);
    gtk_widget_set_size_request(create_new_chat_btn, 21, 21);

    gtk_widget_set_margin_start(create_new_chat_btn, 305);

    widget_styling(chats_box, current_screen, "mini_groups_box");
    widget_styling(chats_label, current_screen, "group_label");
    widget_styling(create_new_chat_btn, current_screen, "create_new_chat_btn");
    // widget_styling(chats_list_grid_scrolled, current_screen, "chats_list_grid");

    gtk_box_append(GTK_BOX(chats_box), chats_label);
    gtk_box_append(GTK_BOX(chats_box), create_new_chat_btn);

    // create new chat on + click
    g_signal_connect(create_new_chat_btn, "clicked", G_CALLBACK(create_new_chat_clicked), NULL);

    gtk_grid_attach(GTK_GRID(current_grid.mini_chats), chats_box, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.mini_chats), chats_list_grid_scrolled, 0, 1, 1, 1);
    // gtk_grid_attach(GTK_GRID(chats_list_grid), chats_list_grid_scrolled, 0, 0, 1, 1);
}
