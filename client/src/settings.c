#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void settings_first_scheme_btn_click() {
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

void settings_second_scheme_btn_click() {
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/purple_theme.css");
}

void settings_third_scheme_btn_click() {
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/blue_theme.css");
}

GtkWidget *create_color_circle(const gchar *const style) {
    GtkWidget *circle = gtk_button_new();
    gtk_widget_set_size_request(circle, 35, 35);
    widget_styling(circle, curent_screen, style);

    return circle;
}

void show_settings() {
    GtkWidget *settings_grid_with_ico = create_grid(200, 200, "settings_grid_with_ico");
    GtkWidget *settings_grid_with_lab = create_grid(160, 35, "settings_grid_with_lab");
    GtkWidget *settings_grid_with_body = create_grid(470, 340, "settings_grid_with_body");
    GtkWidget *settings_grid_with_color_scheme = create_grid(190, 55, "settings_grid_with_color_scheme");

    GtkWidget *settings_lab = gtk_label_new("Settings");

    GtkWidget *settings_first_scheme_btn = create_color_circle("settings_first_scheme_btn");
    GtkWidget *settings_second_scheme_btn = create_color_circle("settings_second_scheme_btn");
    GtkWidget *settings_third_scheme_btn = create_color_circle("settings_third_scheme_btn");

    gtk_grid_attach(GTK_GRID(curent_grid.settings), settings_grid_with_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.settings), settings_grid_with_lab, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.settings), settings_grid_with_body, 0, 2, 1, 1);

    widget_styling(settings_lab, curent_screen, "settings_lab");

    gtk_grid_attach(GTK_GRID(settings_grid_with_lab), settings_lab, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_color_scheme, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), settings_first_scheme_btn, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), settings_second_scheme_btn, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), settings_third_scheme_btn, 2, 0, 1, 1);

    g_signal_connect(settings_first_scheme_btn, "clicked", G_CALLBACK(settings_first_scheme_btn_click), NULL);
    g_signal_connect(settings_second_scheme_btn, "clicked", G_CALLBACK(settings_second_scheme_btn_click), NULL);
    g_signal_connect(settings_third_scheme_btn, "clicked", G_CALLBACK(settings_third_scheme_btn_click), NULL);
}
