#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void show_achievements() {
    //change lab to your code
    GtkWidget *scrolled_window = gtk_label_new("ACHIEVEMENTS");
    //your main box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);;

    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    //attach your main box to mygrid
    gtk_grid_attach(GTK_GRID(curent_grid.achievements), main_box, 0, 0, 1, 1);
    //show to on screen
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.achievements), TRUE);
}
