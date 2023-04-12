#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;


void show_empty_chat() {

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *label = gtk_label_new("Select a chat or group");

    gtk_box_append(GTK_BOX(box), label);

    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_vexpand(label, TRUE);

    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, TRUE);

    gtk_widget_set_size_request(box, 200, 36);

    gtk_grid_attach(GTK_GRID(current_grid.empty_chat), box, 0, 0, 1, 1);

    widget_styling(box, current_screen, "empty_chat_box");
    widget_styling(label, current_screen, "empty_chat_label");

}
