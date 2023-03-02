#include "../inc/client.h"

extern t_screen curent_screen;
t_grid curent_grid;

void set_unvisible_intro() {
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.first_intro_screen), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.second_intro_screen), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.third_intro_screen), FALSE);
}

void set_first_intro_screen_visible() {
    set_unvisible_intro();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.first_intro_screen), TRUE);
    g_timeout_add_seconds(2, (GSourceFunc)set_second_intro_screen_visible, NULL);
}
void set_second_intro_screen_visible() {
    set_unvisible_intro();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.second_intro_screen), TRUE);
    g_timeout_add_seconds(1, (GSourceFunc)set_third_intro_screen_visible, NULL);
}
void set_third_intro_screen_visible() {
    set_unvisible_intro();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.third_intro_screen), TRUE);
}

void first_intro_screen() {
    GtkWidget *welcome = gtk_label_new("Welcome to the dark!");
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_widget_set_margin_start(welcome, 103);
    gtk_widget_set_margin_end(welcome, 99);
    gtk_widget_set_margin_top(welcome, 60);

    gtk_widget_set_size_request(box, 586, 544);
    gtk_box_append(GTK_BOX(box), welcome);

    widget_styling(box, curent_screen, "intro_welcome_to_the_dark");
    gtk_grid_attach(GTK_GRID(curent_grid.first_intro_screen), box, 0, 0, 1, 1);
}

void second_intro_screen() {
    GtkWidget *welcome = gtk_label_new_with_mnemonic ("second_intro_screen");
    gtk_grid_attach(GTK_GRID(curent_grid.second_intro_screen), welcome, 0, 0, 1, 1);
}

static void intro_btn_clicked () {
    set_unvisible_all();
    curent_grid.is_log_in_clicked = FALSE;
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.home), TRUE);
}

void third_intro_screen() {
    GtkWidget *lab = gtk_label_new_with_mnemonic ("We now have your photo!");
    GtkWidget *intro_btn = gtk_button_new_with_mnemonic("Continue");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(lab, 103);
    gtk_widget_set_margin_end(lab, 99);
    gtk_widget_set_margin_top(lab, 60);

    gtk_widget_set_size_request(box, 586, 544);
    gtk_box_append(GTK_BOX(box), lab);
    gtk_box_append(GTK_BOX(box), intro_btn);

    widget_styling(box, curent_screen, "intro_third_box");
    widget_styling(lab, curent_screen, "auth_welcome_to_the_dark");
    widget_styling(intro_btn, curent_screen, "intro_button");
    gtk_grid_attach(GTK_GRID(curent_grid.third_intro_screen), box, 0, 0, 1, 1);

    g_signal_connect(intro_btn, "clicked", G_CALLBACK(intro_btn_clicked), NULL);
}
