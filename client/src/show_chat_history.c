#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

void show_chat_history(t_list *chat_history)
{
    int pos = 0;

    t_list *current = chat_history;

    while (current)
    {
        pos++;

        GtkWidget *child = gtk_widget_get_last_child(current_grid.empty_chat);

        gtk_widget_unparent(child);

        // GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        // GtkWidget *recieved_msg = gtk_text_view_new();

        GtkWidget *sent_msg = gtk_text_view_new();

        const char *s_msg = ((t_chat *)current->data)->message;

        // const char *r_msg =

        // gtk_box_append(GTK_BOX(box), sent_msg);

        // gtk_box_append(GTK_BOX(box), recieved_msg);

        GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);

        gtk_text_buffer_set_text(buffer, s_msg, -1);

        gtk_text_view_set_buffer(GTK_TEXT_VIEW(sent_msg), buffer);

        gtk_text_view_set_editable(GTK_TEXT_VIEW(sent_msg), false);

        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(sent_msg), false);

        gtk_widget_set_halign(sent_msg, GTK_ALIGN_END);

        gtk_widget_set_size_request(sent_msg, 365, 40);

        // int pos = ((t_chat *)current->data)->id;

        gtk_grid_attach(GTK_GRID(current_grid.empty_chat), sent_msg, 0, pos, 1, 1);

        current = current->next;
    }
}
