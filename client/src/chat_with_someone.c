#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

GtkWidget *sent_msg(const char *msg) {
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_END);

    GtkWidget *message_label = gtk_label_new(msg);

    gtk_label_set_wrap(GTK_LABEL(message_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(message_label), PANGO_WRAP_WORD_CHAR);

    gtk_grid_attach(GTK_GRID(grid), message_label, 0, 0, 1, 1);

    // Set the minimal width of the grid
    gtk_widget_set_size_request(grid, 100, -1);

//    widget_styling(grid, current_screen, "right_msg");

    return grid;
}


GtkWidget *recieve_msg(const char *msg) {
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_START);

    GtkWidget *message_label = gtk_label_new(msg);

    gtk_label_set_wrap(GTK_LABEL(message_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(message_label), PANGO_WRAP_WORD_CHAR);

    gtk_grid_attach(GTK_GRID(grid), message_label, 0, 0, 1, 1);

    // Set the minimal width of the grid
    gtk_widget_set_size_request(grid, 100, -1);

//    widget_styling(grid, current_screen, "left_msg");

    return grid;
}

void show_chats() {
//    current_grid.chat_pos_count = 0;

    current_grid.chats_scrolled = create_grid(557, 513, "chats_list_grid");

    //scroll
    GtkWidget *chats_list_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chats_list_grid_scrolled), current_grid.chats_scrolled);

    gtk_widget_set_size_request(chats_list_grid_scrolled, 570, 513);


    gtk_grid_attach(GTK_GRID(current_grid.chats), chats_list_grid_scrolled, 0, 0, 1, 1);


    GtkWidget *sent_msg_widget;
//    GtkWidget *rec_msg_widget;
//    sent_msg_widget = sent_msg("sent");
//    gtk_grid_attach(GTK_GRID(current_grid.chats_scrolled), sent_msg_widget, 0, i, 1, 1);
//
//    rec_msg_widget = recieve_msg("recieve");
//
//    gtk_grid_attach(GTK_GRID(current_grid.chats_scrolled), rec_msg_widget, 0, j++, 1, 1);

    for (int i = 0; i <= 50; i++){
        sent_msg_widget = sent_msg("sent");
        gtk_grid_attach(GTK_GRID(current_grid.chats_scrolled), sent_msg_widget, 0, i, 1, 1);
    }

    widget_styling(chats_list_grid_scrolled, current_screen, "settings_grid_with_body");



    GtkWidget *message_entry = gtk_entry_new();
    gtk_widget_set_size_request(message_entry, 452, 40);

    gtk_grid_attach(GTK_GRID(current_grid.chats), message_entry, 0, 1, 1, 1);

//    g_signal_connect(G_OBJECT(message_entry), "activate", G_CALLBACK(send_message), message_entry);

}
