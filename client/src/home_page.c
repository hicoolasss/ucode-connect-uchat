#include "../inc/client.h"

extern t_screen curent_screen;

void show_home_page_screen(void) {

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
    GtkWidget *search_bar = gtk_search_bar_new();

    GtkWidget *search_btn = gtk_button_new();

    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), " Search");





    //groups box
    GtkWidget *groups_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    //chats box
    GtkWidget *chats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    //scrollbar
    GtkWidget *scrollbar = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, NULL);

    //box with randomly generated sentences
    GtkWidget *current_dirrect_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);



    gtk_window_set_child(GTK_WINDOW(curent_screen.screen), main_grid);

    //main_menu_box(all boxes has 1 style) init
    gtk_grid_attach(GTK_GRID(main_grid), main_menu_box, 0, 0, 1, 1);

    gtk_widget_set_halign(main_menu_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_menu_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(main_menu_box, 33);
    gtk_widget_set_margin_end(main_menu_box, 1043);
    gtk_widget_set_margin_top(main_menu_box, 45);
    gtk_widget_set_margin_bottom(main_menu_box, 45);

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
    gtk_widget_set_margin_start(home_btn, 47);
    gtk_widget_set_margin_end(home_btn, 46);
    gtk_widget_set_margin_top(home_btn, 52);

    gtk_widget_set_size_request(home_btn, 32, 32);

    //chats_btn
    gtk_widget_set_margin_start(chats_btn, 46);
    gtk_widget_set_margin_end(chats_btn, 46);
    gtk_widget_set_margin_top(chats_btn, 54);

    gtk_widget_set_size_request(chats_btn, 32, 32);

    //settings_btn
    gtk_widget_set_margin_start(settings_btn, 47);
    gtk_widget_set_margin_end(settings_btn, 46);
    gtk_widget_set_margin_top(settings_btn, 54);

    gtk_widget_set_size_request(settings_btn, 32, 32);

    //achievements_btn
    gtk_widget_set_margin_start(achievements_btn, 47);
    gtk_widget_set_margin_end(achievements_btn, 46);
    gtk_widget_set_margin_top(achievements_btn, 54);

    gtk_widget_set_size_request(achievements_btn, 32, 32);

    //log_out_btn
    gtk_widget_set_margin_start(log_out_btn, 47);
    gtk_widget_set_margin_end(log_out_btn, 46);
    gtk_widget_set_margin_top(log_out_btn, 160);

    gtk_widget_set_size_request(log_out_btn, 32, 32);

    
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

    gtk_widget_set_margin_start(search_bar_box, 177);
    gtk_widget_set_margin_end(search_bar_box, 596);
    gtk_widget_set_margin_top(search_bar_box, 45);
    gtk_widget_set_margin_bottom(search_bar_box, 656);

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
    gtk_widget_set_margin_start(entry_for_search, 31);
    gtk_widget_set_margin_top(entry_for_search, 15);
    gtk_widget_set_margin_bottom(entry_for_search, 15);

    gtk_widget_set_size_request(entry_for_search, 57, 24);



    //styling
    widget_styling(search_bar_box, curent_screen, "main_box_style");
    widget_styling(search_btn, curent_screen, "search_btn");
    widget_styling(entry_for_search, curent_screen, "entry_for_search");












    //groups_box init
    gtk_grid_attach(GTK_GRID(main_grid), groups_box, 0, 0, 1, 1);

    gtk_widget_set_halign(groups_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(groups_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(groups_box, 177);
    gtk_widget_set_margin_end(groups_box, 596);
    gtk_widget_set_margin_top(groups_box, 127);
    gtk_widget_set_margin_bottom(groups_box, 315);

    gtk_widget_set_size_request(groups_box, 427, 318);

    //styling
    widget_styling(groups_box, curent_screen, "main_box_style");

    //chats_box init
    gtk_grid_attach(GTK_GRID(main_grid), chats_box, 0, 0, 1, 1);

    gtk_widget_set_halign(chats_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(chats_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(chats_box, 177);
    gtk_widget_set_margin_end(chats_box, 596);
    gtk_widget_set_margin_top(chats_box, 468);
    gtk_widget_set_margin_bottom(chats_box, 46);

    gtk_widget_set_size_request(chats_box, 427, 246);

    //styling
    widget_styling(chats_box, curent_screen, "main_box_style");




    /*
    

            space for scrollbar
    
    
    
    */


    //current_dirrect_message_box init
    gtk_grid_attach(GTK_GRID(main_grid), current_dirrect_message_box, 0, 0, 1, 1);

    gtk_widget_set_halign(current_dirrect_message_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(current_dirrect_message_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(current_dirrect_message_box, 656);
    gtk_widget_set_margin_end(current_dirrect_message_box, 31);
    gtk_widget_set_margin_top(current_dirrect_message_box, 47);
    gtk_widget_set_margin_bottom(current_dirrect_message_box, 46);

    gtk_widget_set_size_request(current_dirrect_message_box, 533, 667);

    //styling
    widget_styling(current_dirrect_message_box, curent_screen, "main_box_style");








}

