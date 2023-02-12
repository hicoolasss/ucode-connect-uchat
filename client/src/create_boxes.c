#include "../inc/client.h"

extern t_screen curent_screen;

GtkWidget *create_hbox( const gint width,
                        const gint height,
                        const gchar *const style ) {
    GtkWidget *child = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(child, width, height);
    if (style != NULL)
        widget_styling(child, curent_screen, style);

    return child;
}

GtkWidget *create_vbox( const gint width,
                        const gint height,
                        const gchar *const style ) {
    GtkWidget *child = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(child, width, height);
    if (style != NULL)
        widget_styling(child, curent_screen, style);

    return child;
}
