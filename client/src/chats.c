#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

void show_chats() {

    GtkWidget *chats_container_lab = gtk_label_new("chats");
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);;

    gtk_widget_set_size_request(main_box, 553, 667);
    
    gtk_box_append(GTK_BOX(main_box), chats_container_lab);

    gtk_grid_attach(GTK_GRID(curent_grid.chats), main_box, 0, 0, 1, 1); 

    widget_styling(main_box, curent_screen, "main_box_style");
}

void show_search_bar() {

    //box for search_bar
    GtkWidget *search_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //search bar on top of the screen
    //GtkWidget *search_bar = gtk_search_bar_new();

    GtkWidget *search_btn = gtk_button_new();

    GtkWidget *entry_for_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_search), "Search");

    //search_bar_box init

    gtk_widget_set_halign(search_bar_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(search_bar_box, GTK_ALIGN_CENTER);

    
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
//    gtk_widget_set_margin_start(entry_for_search, 15);
//    gtk_widget_set_margin_top(entry_for_search, 15);
//    gtk_widget_set_margin_bottom(entry_for_search, 15);

    gtk_widget_set_size_request(entry_for_search, 357, 21);



    //styling
    widget_styling(search_btn, curent_screen, "search_btn");
    widget_styling(entry_for_search, curent_screen, "entry_for_search");



    gtk_grid_attach(GTK_GRID(curent_grid.search_bar), search_bar_box, 0, 0, 1, 1);
}

void show_mini_chats() {

    GtkWidget *chats_label = gtk_label_new("Chats");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_size_request(main_box, 427, 246);
    
    gtk_box_append(GTK_BOX(main_box), chats_label);

    gtk_grid_attach(GTK_GRID(curent_grid.mini_chats), main_box, 0, 0, 1, 1);

//    widget_styling(main_box, curent_screen, "chats");


}

void show_mini_groups() {

    GtkWidget *groups_label = gtk_label_new("Groups");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_size_request(main_box, 427, 318);
    
    gtk_box_append(GTK_BOX(main_box), groups_label);

    gtk_grid_attach(GTK_GRID(curent_grid.mini_groups), main_box, 0, 0, 1, 1);

//    widget_styling(main_box, curent_screen, "chats");
}
