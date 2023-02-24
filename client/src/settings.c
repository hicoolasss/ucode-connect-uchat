#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void change_scheme_to_any_color(char *color) {
    FILE *css_file = fopen("resources/style/color.css", "w");
    char our_final_line[1024] = "@define-color my-main-color ";
    strcat(our_final_line, color);
    strcat(our_final_line, ";");

    fprintf(css_file, "%s\n", our_final_line);
    fclose(css_file);
}

void settings_first_scheme_btn_click() {
    change_scheme_to_any_color("#0ff");
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

void settings_second_scheme_btn_click() {
    change_scheme_to_any_color("#9400d3");
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

void settings_third_scheme_btn_click() {
    change_scheme_to_any_color("#00ff00");
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

GtkWidget *create_color_circle(const gchar *const style) {
    GtkWidget *circle = gtk_button_new();
    gtk_widget_set_size_request(circle, 35, 35);
    widget_styling(circle, curent_screen, style);

    return circle;
}

void show_settings() {

    GtkWidget *settings_grid_help = create_grid(470, 629, "settings_grid_help");
    GtkWidget *settings_grid_for_ico = create_grid(201, 201, "settings_grid_for_ico");
    GtkWidget *settings_grid_with_ico = create_grid(200, 200, "settings_grid_with_ico");
    GtkWidget *settings_grid_with_lab = create_grid(160, 35, "settings_grid_with_lab");
    GtkWidget *settings_grid_with_body = create_grid(470, 340, "settings_grid_with_body");
    GtkWidget *settings_grid_with_color_scheme = create_grid(190, 85, "settings_grid_with_color_scheme");
    GtkWidget *settings_grid_for_new_lab = create_grid(190, 35, NULL);
    GtkWidget *settings_grid_with_color_scheme_list = create_grid(190, 55, NULL);
    GtkWidget *settings_grid_with_color_choser = create_grid(190, 85, "settings_grid_with_color_scheme");

    GtkWidget *color_scheme_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(color_scheme_grid_scrolled), settings_grid_with_color_scheme_list);
    gtk_widget_set_size_request(color_scheme_grid_scrolled, 203, 55);

    GtkWidget *settings_lab = gtk_label_new("Settings");
    GtkWidget *settings_lab_nsh = gtk_label_new("Create new theme");
    GtkWidget *settings_lab_nsh_plus = gtk_button_new_with_label("+");


    GtkWidget *settings_first_scheme_btn = create_color_circle("settings_new_scheme_btn");
    GtkWidget *settings_second_scheme_btn = create_color_circle("settings_new_scheme_btn");
    GtkWidget *settings_third_scheme_btn = create_color_circle("settings_new_scheme_btn");

    gtk_grid_attach(GTK_GRID(curent_grid.settings), settings_grid_help, 0, 0, 1, 1);
    gtk_grid_set_row_spacing(GTK_GRID(settings_grid_help), 19);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_for_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_for_ico), settings_grid_with_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_with_lab, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_with_body, 0, 2, 1, 1);

    gtk_widget_set_size_request(settings_lab_nsh, 150, 30);
    gtk_widget_set_size_request(settings_lab_nsh_plus, 30, 30);

    widget_styling(settings_lab, curent_screen, "settings_lab");
    widget_styling(settings_lab_nsh, curent_screen, "settings_lab_nsh");
    widget_styling(settings_lab_nsh_plus, curent_screen, "settings_lab_nsh_plus");

    gtk_grid_attach(GTK_GRID(settings_grid_with_lab), settings_lab, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_color_scheme, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_color_choser, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), settings_grid_for_new_lab, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), color_scheme_grid_scrolled, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_for_new_lab), settings_lab_nsh, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_for_new_lab), settings_lab_nsh_plus, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_first_scheme_btn, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_second_scheme_btn, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_third_scheme_btn, 2, 0, 1, 1);

    g_signal_connect(settings_first_scheme_btn, "clicked", G_CALLBACK(settings_first_scheme_btn_click), NULL);
    g_signal_connect(settings_second_scheme_btn, "clicked", G_CALLBACK(settings_second_scheme_btn_click), NULL);
    g_signal_connect(settings_third_scheme_btn, "clicked", G_CALLBACK(settings_third_scheme_btn_click), NULL);
}

