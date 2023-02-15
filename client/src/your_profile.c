#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void show_your_profile() {
    GtkWidget *your_profile_lab = gtk_label_new("YOUR PROFILE");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);;

    gtk_box_append(GTK_BOX(main_box), your_profile_lab);

    gtk_grid_attach(GTK_GRID(curent_grid.your_profile), main_box, 0, 0, 1, 1);
}
