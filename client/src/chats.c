#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

static void send_message(GtkEntry *message_entry, gpointer user_data) {

        GtkEntryBuffer *buffer = gtk_entry_get_buffer(message_entry);
        const char *buf = gtk_entry_buffer_get_text(buffer);
        char *temp_buffer = mx_strdup(buf);
        

        int len = SSL_write(cur_client.ssl, temp_buffer, mx_strlen(temp_buffer));

        if (len < 0)
        {
            mx_printstr("Error sending message.\n");
        }

        memset(temp_buffer, 0, sizeof(*temp_buffer));

        GtkWidget *label = gtk_text_new_with_buffer(buffer);
        
        gtk_grid_attach(GTK_GRID(curent_grid.chats), label, 0, 0, 1, 1);

}

void show_chats() {

    GtkWidget *chats_container_lab = gtk_label_new("chats");

    gtk_grid_attach(GTK_GRID(curent_grid.chats), chats_container_lab, 0, 0, 1, 1);

    GtkWidget *message_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(curent_grid.chats), message_entry, 0, 0, 1, 1);

    //gtk_entry_set_alignment(GTK_ENTRY(first_name_entry), 0.1);

    gtk_widget_set_margin_start(message_entry, 26);
    gtk_widget_set_margin_end(message_entry, 55);
    gtk_widget_set_margin_top(message_entry, 613);
    gtk_widget_set_margin_bottom(message_entry, 14);

    gtk_widget_set_size_request(message_entry, 452, 40);

    g_signal_connect(G_OBJECT(message_entry), "activate", G_CALLBACK(send_message), NULL);

}

void call_new_chat_and_add_iter(const gchar *const new_username){
    create_new_chat(curent_grid.chat_pos_count,
                    new_username);

    curent_grid.chat_pos_count++;
}

static void create_new_chat_clicked() {
//    search_btn_clicked();
//    call_new_chat_and_add_iter();
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
