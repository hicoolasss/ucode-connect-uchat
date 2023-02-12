#include "../inc/client.h"

extern t_screen curent_screen;

GtkWidget *create_grid( const gint width,
                        const gint height,
                        const gchar *const style ) {
    GtkWidget *child = gtk_grid_new();
    gtk_widget_set_size_request(child, width, height);
    if (style != NULL)
        widget_styling(child, curent_screen, style);

    return child;
}
