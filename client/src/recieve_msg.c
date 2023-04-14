#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_achievements current_achievements;

void receive_msg(char *msg)
{

    GtkWidget *previous_msg = gtk_widget_get_last_child(current_grid.chat_with_friend);

    GtkWidget *recieved_msg = gtk_label_new(msg);

    gtk_widget_set_halign(recieved_msg, GTK_ALIGN_START);

    gtk_widget_set_margin_top(recieved_msg, 15);

    gtk_label_set_wrap(GTK_LABEL(recieved_msg), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(recieved_msg), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_max_width_chars(GTK_LABEL(recieved_msg), 50);
    gtk_label_set_selectable(GTK_LABEL(recieved_msg), FALSE);

    gtk_widget_set_hexpand(recieved_msg, TRUE);

    widget_styling(recieved_msg, current_screen, "message");
    gtk_grid_insert_next_to(GTK_GRID(current_grid.chat_with_friend), previous_msg, GTK_POS_BOTTOM);

    // Add the received_msg to the grid
    gtk_grid_attach_next_to(GTK_GRID(current_grid.chat_with_friend), recieved_msg, previous_msg, GTK_POS_BOTTOM, 1, 1);
}
