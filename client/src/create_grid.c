#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

//function to create grid containers
GtkWidget *create_grid( const gint width,
                        const gint height,
                        const gchar *const style ) {
    GtkWidget *child = gtk_grid_new();
    gtk_widget_set_size_request(child, width, height);
    if (style != NULL)
        widget_styling(child, current_screen, style);

    return child;
}
