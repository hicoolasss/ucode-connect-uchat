#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_avatar current_avatar;
extern t_scaled_avatar current_scaled_avatar;

void get_scaled_image() {
   
   //GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(current_avatar.avatar, NULL);
    if (!current_avatar.avatar) {
        g_print("Ошибка при загрузке изображения.\n");
        return;
    }

    // Масштабирование исходного изображения до размера аватара
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(current_avatar.avatar, 67, 67, GDK_INTERP_BILINEAR);
    //g_object_unref(current_avatar.avatar);

    // Создание поверхности Cairo для рисования
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 67, 67);
    cairo_t *cr = cairo_create(surface);

    // Создание круглой области
    cairo_arc(cr, 67 / 2.0, 67 / 2.0, 67 / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);
    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    GdkPixbuf *circle_pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 67, 67);

    current_scaled_avatar.scaled_avatar = circle_pixbuf;
    
}


void your_profile_clicked () {
    set_unvisible_all();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.your_profile), TRUE);
}

void home_clicked () {
    set_unvisible_all();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.home), TRUE);
}

void chats_clicked () {
    set_unvisible_all();

    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats_container), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.empty_chat), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chat_with_friend), FALSE);
}

void settings_clicked () {
    set_unvisible_all();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.settings), TRUE);
}

void achievements_clicked () {
    set_unvisible_all();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.achievements), TRUE);
}

void log_out_clicked () {
    show_log_out();
}

void show_left_menu_bar() {

    current_avatar.avatar = gdk_pixbuf_new_from_file("avatar1.png", NULL);

    get_scaled_image();

    GtkWidget *profile_avatar_icon = gtk_image_new_from_pixbuf(current_scaled_avatar.scaled_avatar);
    GtkWidget *profile_avatar_btn = gtk_button_new();
    gtk_widget_set_margin_start(profile_avatar_icon, 0);
    gtk_widget_set_margin_end(profile_avatar_icon, 0);
    gtk_widget_set_margin_top(profile_avatar_icon, 0);
    gtk_widget_set_margin_bottom(profile_avatar_icon, 0);
    gtk_button_set_child(GTK_BUTTON(profile_avatar_btn), profile_avatar_icon);


    GtkWidget *home_btn = gtk_button_new();
    GtkWidget *chats_btn = gtk_button_new();
    GtkWidget *settings_btn = gtk_button_new();
    GtkWidget *achievements_btn = gtk_button_new();
    GtkWidget *log_out_btn = gtk_button_new();

    gtk_widget_set_halign(current_grid.left_menu_bar, GTK_ALIGN_START);
    //gtk_widget_set_valign(current_grid.left_menu_bar, GTK_ALIGN_CENTER);

    gtk_widget_set_hexpand(current_grid.left_menu_bar, TRUE);
    gtk_widget_set_vexpand(current_grid.left_menu_bar, TRUE);

    //attach to side
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), profile_avatar_btn, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), home_btn, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), chats_btn, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), settings_btn, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), achievements_btn, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.left_menu_bar), log_out_btn, 0, 5, 1, 1);

    //profile_avatar_btn
    gtk_widget_set_margin_start(profile_avatar_btn, 29);
    gtk_widget_set_margin_end(profile_avatar_btn, 29);
    gtk_widget_set_margin_top(profile_avatar_btn, 35);

    gtk_widget_set_size_request(profile_avatar_btn, 67, 67);

    //home_btn
    gtk_widget_set_margin_start(home_btn, 17);
    gtk_widget_set_margin_end(home_btn, 16);
    gtk_widget_set_margin_top(home_btn, 37);

    gtk_widget_set_size_request(home_btn, 62, 62);

    //chats_btn
    gtk_widget_set_margin_start(chats_btn, 17);
    gtk_widget_set_margin_end(chats_btn, 16);
    gtk_widget_set_margin_top(chats_btn, 37);

    gtk_widget_set_size_request(chats_btn, 62, 62);

    //settings_btn
    gtk_widget_set_margin_start(settings_btn, 17);
    gtk_widget_set_margin_end(settings_btn, 16);
    gtk_widget_set_margin_top(settings_btn, 37);

    gtk_widget_set_size_request(settings_btn, 62, 62);

    //achievements_btn
    gtk_widget_set_margin_start(achievements_btn, 17);
    gtk_widget_set_margin_end(achievements_btn, 16);
    gtk_widget_set_margin_top(achievements_btn, 37);

    gtk_widget_set_size_request(achievements_btn, 62, 62);

    //log_out_btn
    gtk_widget_set_margin_start(log_out_btn, 17);
    gtk_widget_set_margin_end(log_out_btn, 16);
    gtk_widget_set_margin_top(log_out_btn, 90);

    gtk_widget_set_size_request(log_out_btn, 62, 62);

    //styling
//    widget_styling(main_menu_box, current_screen, "main_box_style");
    widget_styling(current_grid.left_menu_bar, current_screen, "main_box_style");
    widget_styling(profile_avatar_btn, current_screen, "profile_avatar_btn");
    widget_styling(home_btn, current_screen, "home_btn");
    widget_styling(chats_btn, current_screen, "chats_btn");
    widget_styling(settings_btn, current_screen, "settings_btn");
    widget_styling(achievements_btn, current_screen, "achievements_btn");
    widget_styling(log_out_btn, current_screen, "log_out_btn");


    //signals
    g_signal_connect(profile_avatar_btn, "clicked", G_CALLBACK(your_profile_clicked), NULL);
    g_signal_connect(home_btn, "clicked", G_CALLBACK(home_clicked), NULL);
    g_signal_connect(chats_btn, "clicked", G_CALLBACK(chats_clicked), NULL);
    g_signal_connect(settings_btn, "clicked", G_CALLBACK(settings_clicked), NULL);
    g_signal_connect(achievements_btn, "clicked", G_CALLBACK(achievements_clicked), NULL);
    g_signal_connect(log_out_btn, "clicked", G_CALLBACK(log_out_clicked), NULL);

}
