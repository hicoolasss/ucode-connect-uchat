#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void show_home_grid() {
    GtkWidget *home_lab = gtk_label_new("HOME");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_size_request(main_box, 1010, 667);

    gtk_box_append(GTK_BOX(main_box), home_lab);
    gtk_grid_attach(GTK_GRID(curent_grid.home), main_box, 0, 0, 1, 1);
}
