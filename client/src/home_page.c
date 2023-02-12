#include "../inc/client.h"

extern t_screen curent_screen;

t_grid curent_grid;

static void log_out_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_log_out();
}

static void settings_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_settings();
}

static void achievements_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_achievements();
}

void show_home(void) {

    //main box

    GtkWidget *main_grid = gtk_grid_new();
    
    //left box(home,settings,profile,etc..)
    GtkWidget *main_menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *profile_avatar_btn = gtk_button_new();

    GtkWidget *home_btn = gtk_button_new();

    GtkWidget *chats_btn = gtk_button_new();

    GtkWidget *settings_btn = gtk_button_new();

    GtkWidget *achievements_btn = gtk_button_new();

    GtkWidget *log_out_btn = gtk_button_new();


    //box for search_bar
    GtkWidget *search_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //search bar on top of the screen
    //GtkWidget *search_bar = gtk_search_bar_new();

    GtkWidget *search_btn = gtk_button_new();

    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), "Search");


    //groups box
    GtkWidget *groups_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *groups_label = gtk_label_new("Groups");

    GtkWidget *create_new_group_btn = gtk_button_new();


    //chats box
    GtkWidget *chats_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *chats_label = gtk_label_new("Chats");

    GtkWidget *create_new_chat_btn = gtk_button_new();


    //current_dirrect_message_box
    GtkWidget *current_dirrect_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *lets_start = gtk_label_new("LET'S START");




    gtk_window_set_child(GTK_WINDOW(curent_screen.screen), main_grid);

    //main_menu_box(all boxes has 1 style) init
    gtk_grid_attach(GTK_GRID(main_grid), main_menu_box, 0, 0, 1, 1);

    gtk_widget_set_halign(main_menu_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_menu_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(main_menu_box, 22);
    gtk_widget_set_margin_end(main_menu_box, 1054);
    gtk_widget_set_margin_top(main_menu_box, 47);
    gtk_widget_set_margin_bottom(main_menu_box, 46);

    gtk_widget_set_size_request(main_menu_box, 124, 667);

    //
    gtk_box_append(GTK_BOX(main_menu_box), profile_avatar_btn);
    gtk_box_append(GTK_BOX(main_menu_box), home_btn);
    gtk_box_append(GTK_BOX(main_menu_box), chats_btn);
    gtk_box_append(GTK_BOX(main_menu_box), settings_btn);
    gtk_box_append(GTK_BOX(main_menu_box), achievements_btn);
    gtk_box_append(GTK_BOX(main_menu_box), log_out_btn);



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
    widget_styling(main_menu_box, curent_screen, "main_box_style");
    widget_styling(profile_avatar_btn, curent_screen, "profile_avatar_btn");
    widget_styling(home_btn, curent_screen, "home_btn");
    widget_styling(chats_btn, curent_screen, "chats_btn");
    widget_styling(settings_btn, curent_screen, "settings_btn");
    widget_styling(achievements_btn, curent_screen, "achievements_btn");
    widget_styling(log_out_btn, curent_screen, "log_out_btn");



    //search_bar_box init
    gtk_grid_attach(GTK_GRID(main_grid), search_bar_box, 0, 0, 1, 1);

    gtk_widget_set_halign(search_bar_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(search_bar_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(search_bar_box, 170);
    gtk_widget_set_margin_end(search_bar_box, 603);
    gtk_widget_set_margin_top(search_bar_box, 47);
    gtk_widget_set_margin_bottom(search_bar_box, 654);

    gtk_widget_set_size_request(search_bar_box, 427, 59);



    //packing search_bar_btn into search_bar_box
    gtk_box_append(GTK_BOX(search_bar_box), search_btn);
    gtk_box_append(GTK_BOX(search_bar_box), entry_for_search);

    //search_btn
    gtk_widget_set_margin_start(search_btn, 23);
    gtk_widget_set_margin_top(search_btn, 15);
    gtk_widget_set_margin_bottom(search_btn, 15);

    gtk_widget_set_size_request(search_btn, 24, 24);

    //entry_for_search
    gtk_widget_set_margin_start(entry_for_search, 15);
    gtk_widget_set_margin_top(entry_for_search, 15);
    gtk_widget_set_margin_bottom(entry_for_search, 15);

    gtk_widget_set_size_request(entry_for_search, 357, 21);



    //styling
    widget_styling(search_bar_box, curent_screen, "search_bar_box");
    widget_styling(search_btn, curent_screen, "search_btn");
    widget_styling(entry_for_search, curent_screen, "entry_for_search");



    //groups_box init
    gtk_grid_attach(GTK_GRID(main_grid), groups_box, 0, 0, 1, 1);

    gtk_widget_set_halign(groups_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(groups_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(groups_box, 170);
    gtk_widget_set_margin_end(groups_box, 603);
    gtk_widget_set_margin_top(groups_box, 128);
    gtk_widget_set_margin_bottom(groups_box, 350);

    gtk_widget_set_size_request(groups_box, 427, 282);



    //groups_label
    gtk_box_append(GTK_BOX(groups_box), groups_label);

    gtk_widget_set_margin_start(groups_label, 20);
    gtk_widget_set_margin_top(groups_label, 12);
    gtk_widget_set_margin_bottom(groups_label, 243);

    gtk_widget_set_size_request(groups_label, 74, 27);

    //create_new_group_btn
    gtk_box_append(GTK_BOX(groups_box), create_new_group_btn);

    gtk_widget_set_margin_start(create_new_group_btn, 288);
    gtk_widget_set_margin_top(create_new_group_btn, 12);
    gtk_widget_set_margin_bottom(create_new_group_btn, 244);
    

    gtk_widget_set_size_request(create_new_group_btn, 32, 32);

    //styling
    widget_styling(groups_box, curent_screen, "main_box_style");
    widget_styling(groups_label, curent_screen, "groups_label");
    widget_styling(create_new_group_btn, curent_screen, "create_new_group_btn");




    //chats_box init
    gtk_grid_attach(GTK_GRID(main_grid), chats_box, 0, 0, 1, 1);

    gtk_widget_set_halign(chats_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(chats_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(chats_box, 170);
    gtk_widget_set_margin_end(chats_box, 603);
    gtk_widget_set_margin_top(chats_box, 432);
    gtk_widget_set_margin_bottom(chats_box, 46);

    gtk_widget_set_size_request(chats_box, 427, 282);


    //chats_label
    gtk_box_append(GTK_BOX(chats_box), chats_label);

    gtk_widget_set_margin_start(chats_label, 20);
    gtk_widget_set_margin_top(chats_label, 11);
    gtk_widget_set_margin_bottom(chats_label, 237);

    gtk_widget_set_size_request(chats_label, 61, 34);


    //create_new_chat_btn
    gtk_box_append(GTK_BOX(chats_box), create_new_chat_btn);

    gtk_widget_set_margin_start(create_new_chat_btn, 301);
    gtk_widget_set_margin_top(create_new_chat_btn, 13);
    gtk_widget_set_margin_bottom(create_new_chat_btn, 237);

    //styling
    widget_styling(chats_box, curent_screen, "main_box_style");
    widget_styling(chats_label, curent_screen, "chats_label");
    widget_styling(create_new_chat_btn, curent_screen, "create_new_chat_btn");



    //current_dirrect_message_box init
    gtk_grid_attach(GTK_GRID(main_grid), current_dirrect_message_box, 0, 0, 1, 1);

    gtk_widget_set_halign(current_dirrect_message_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(current_dirrect_message_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(current_dirrect_message_box, 621);
    gtk_widget_set_margin_end(current_dirrect_message_box, 22);
    gtk_widget_set_margin_top(current_dirrect_message_box, 46);
    gtk_widget_set_margin_bottom(current_dirrect_message_box, 47);

    gtk_widget_set_size_request(current_dirrect_message_box, 557, 667);


    gtk_box_append(GTK_BOX(current_dirrect_message_box), lets_start);

    gtk_widget_set_margin_start(lets_start, 180);
    gtk_widget_set_margin_end(lets_start, 178);
    gtk_widget_set_margin_top(lets_start, 301);

    gtk_widget_set_size_request(lets_start, 200, 36);

    //styling
    widget_styling(current_dirrect_message_box, curent_screen, "current_dirrect_message_box");
    widget_styling(lets_start, curent_screen, "lets_start");


    g_signal_connect(log_out_btn, "clicked", G_CALLBACK(log_out_btn_clicked), NULL);
    g_signal_connect(settings_btn, "clicked", G_CALLBACK(settings_btn_clicked), NULL);
    g_signal_connect(achievements_btn, "clicked", G_CALLBACK(achievements_btn_clicked), NULL);





}

