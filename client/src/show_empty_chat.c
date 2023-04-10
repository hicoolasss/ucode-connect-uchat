#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;


void show_empty_chat() {

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *label = gtk_label_new("Select a chat or group");

    gtk_box_append(GTK_BOX(box), label);

    gtk_widget_set_margin_start(label, 17);
    gtk_widget_set_margin_end(label, 17);
    gtk_widget_set_margin_top(label, 14);
    gtk_widget_set_margin_bottom(box, 14);

    gtk_widget_set_margin_start(box, 179);
    gtk_widget_set_margin_end(box, 178);
    gtk_widget_set_margin_top(box, 317);
    gtk_widget_set_margin_bottom(box, 316);

    gtk_widget_set_size_request(box, 200, 36);

    gtk_grid_attach(GTK_GRID(current_grid.empty_chat), box, 0, 0, 1, 1);

    widget_styling(box, current_screen, "empty_chat_box");
    widget_styling(label, current_screen, "empty_chat_label");

}
