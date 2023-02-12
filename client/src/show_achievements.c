#include "../inc/client.h"

extern t_screen curent_screen;

static void home_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_home();
}

static void settings_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_settings();
}

static void log_out_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_log_out();
}

void show_achievements(void) {

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
    


    //achievements_box
    GtkWidget *achievements_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *first_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *second_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);



    GtkWidget *achievements_icon_btn = gtk_button_new();

    GtkWidget *achievements_label_btn = gtk_button_new_with_label("Achievements");

    //completed and not completed yet buttons
    GtkWidget *complete_btn_0 = gtk_button_new();

    GtkWidget *complete_btn_1 = gtk_button_new();
    
    GtkWidget *complete_btn_2 = gtk_button_new();

    GtkWidget *unfinished_btn_0 = gtk_button_new();

    GtkWidget *unfinished_btn_1 = gtk_button_new();

    GtkWidget *unfinished_btn_2 = gtk_button_new();

    //first box
    GtkWidget *first_step_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *first_step_label = gtk_label_new("First Step!");

    GtkWidget *first_step_desc_label = gtk_label_new("Write your first message");


    //second box
    GtkWidget *explorer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    

    GtkWidget *explorer_label = gtk_label_new("Explorer");

    GtkWidget *explorer_desc_label = gtk_label_new("Open this folder");

    //third box
    GtkWidget *fickle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *fickle_label = gtk_label_new("Fickle");

    GtkWidget *fickle_desc_label = gtk_label_new("Change your avatar");

    //fourth box
    GtkWidget *argument_with_a_women_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *argument_with_a_women_label = gtk_label_new("Argument with a women");

    GtkWidget *argument_with_a_women_desc_label = gtk_label_new("Write 5000 messages");

    //fifth box
    GtkWidget *loving_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *loving_label = gtk_label_new("Loving");

    GtkWidget *loving_desc_label = gtk_label_new("Confess your love");

    //6th box
    GtkWidget *vicious_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *vicious_label = gtk_label_new("vicious");

    GtkWidget *vicious_desc_label = gtk_label_new("Block someone");

    //7th box
    GtkWidget *smart_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *smart_label = gtk_label_new("Loving");

    GtkWidget *smart_desc_label = gtk_label_new("Press achievements button");


    //8th box
    GtkWidget *mission_impossible_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    
    GtkWidget *mission_impossible_label = gtk_label_new("mission_impossible");

    GtkWidget *mission_impossible_desc_label = gtk_label_new("Complete all achievements");

    //scrolled_window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    GtkWidget *scrollbar = gtk_scrollbar_new(GTK_ORIENTATION_VERTICAL, NULL);


    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), achievements_box);
    //gtk_scrolled_window_set_min_content_height(scrolled_window, 500);
    // gtk_scrolled_window_set_hadjustment(scrolled_window, NULL);
    // gtk_scrolled_window_set_min_content_height(scrolled_window, 600);
    gtk_scrolled_window_set_propagate_natural_height (GTK_SCROLLED_WINDOW(scrolled_window),false);
    gtk_scrolled_window_set_propagate_natural_width (GTK_SCROLLED_WINDOW(scrolled_window),true);














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



    //achievements_box init
    gtk_grid_attach(GTK_GRID(main_grid), scrolled_window, 0, 0, 1, 1);

    gtk_widget_set_halign(scrolled_window, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(scrolled_window, 165);
    gtk_widget_set_margin_end(scrolled_window, 25);
    gtk_widget_set_margin_top(scrolled_window, 46);
    gtk_widget_set_margin_bottom(scrolled_window, 47);

    gtk_widget_set_size_request(scrolled_window, 1010, 667);

    //achievements_icon_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_icon_btn);

    gtk_widget_set_margin_start(achievements_icon_btn, 405);
    gtk_widget_set_margin_end(achievements_icon_btn, 405);
    gtk_widget_set_margin_top(achievements_icon_btn, 11);

    gtk_widget_set_size_request(achievements_icon_btn, 200, 200);

    
    //achievements_label_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_label_btn);

    gtk_widget_set_margin_start(achievements_label_btn, 425);
    gtk_widget_set_margin_end(achievements_label_btn, 425);
    gtk_widget_set_margin_top(achievements_label_btn, 32);

    gtk_widget_set_size_request(achievements_label_btn, 130, 40);


    //first_hbox
    gtk_box_append(GTK_BOX(achievements_box), first_hbox);

    gtk_widget_set_halign(first_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(first_hbox, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(first_hbox, 0);
    gtk_widget_set_margin_end(first_hbox, 0);
    gtk_widget_set_margin_top(first_hbox, 32);

    gtk_widget_set_size_request(first_hbox, 1010, 254);

    //first_step_box
    gtk_box_append(GTK_BOX(first_hbox), first_step_box);

    gtk_widget_set_halign(first_step_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(first_step_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(first_step_box, 17);
    gtk_widget_set_margin_top(first_step_box, 0);
    gtk_widget_set_margin_bottom(first_step_box, 0);

    gtk_widget_set_size_request(first_step_box, 190, 254);


    //explorer_box
    gtk_box_append(GTK_BOX(first_hbox), explorer_box);

    gtk_widget_set_halign(explorer_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(explorer_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(explorer_box, 72);
    gtk_widget_set_margin_top(explorer_box, 0);
    gtk_widget_set_margin_bottom(explorer_box, 0);

    gtk_widget_set_size_request(explorer_box, 190, 254);


    //fickle_box
    gtk_box_append(GTK_BOX(first_hbox), fickle_box);

    gtk_widget_set_halign(fickle_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(fickle_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(fickle_box, 72);
    gtk_widget_set_margin_top(fickle_box, 0);
    gtk_widget_set_margin_bottom(fickle_box, 0);

    gtk_widget_set_size_request(fickle_box, 190, 254);


    //argument_with_a_women_box
    gtk_box_append(GTK_BOX(first_hbox), argument_with_a_women_box);

    gtk_widget_set_halign(argument_with_a_women_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(argument_with_a_women_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(argument_with_a_women_box, 72);
    gtk_widget_set_margin_top(argument_with_a_women_box, 0);
    gtk_widget_set_margin_bottom(argument_with_a_women_box, 0);

    gtk_widget_set_size_request(argument_with_a_women_box, 190, 254);


    //second_hbox
    gtk_box_append(GTK_BOX(achievements_box), second_hbox);

    gtk_widget_set_halign(second_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(second_hbox, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(second_hbox, 0);
    gtk_widget_set_margin_end(second_hbox, 0);
    gtk_widget_set_margin_top(second_hbox, 32);
    gtk_widget_set_margin_bottom(second_hbox, 32);

    gtk_widget_set_size_request(second_hbox, 1010, 254);

    //argument_with_a_women_box
    gtk_box_append(GTK_BOX(second_hbox), loving_box);

    gtk_widget_set_halign(loving_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(loving_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(loving_box, 17);
    gtk_widget_set_margin_top(loving_box, 0);
    gtk_widget_set_margin_bottom(loving_box, 0);

    gtk_widget_set_size_request(loving_box, 190, 254);



    



    //styling
    widget_styling(achievements_box, curent_screen, "achievements_box");
    widget_styling(achievements_icon_btn, curent_screen, "achievements_icon_btn");
    widget_styling(achievements_label_btn, curent_screen, "achievements_label_btn");
    widget_styling(first_hbox, curent_screen, "first_hbox");
    widget_styling(first_step_box, curent_screen, "achievement_card");
    widget_styling(explorer_box, curent_screen, "achievement_card");
    widget_styling(fickle_box, curent_screen, "achievement_card");
    widget_styling(argument_with_a_women_box, curent_screen, "achievement_card");

    widget_styling(second_hbox, curent_screen, "second_hbox");
    widget_styling(loving_box, curent_screen, "achievement_card");
    
    


    g_signal_connect(log_out_btn, "clicked", G_CALLBACK(log_out_btn_clicked), NULL);
    g_signal_connect(settings_btn, "clicked", G_CALLBACK(settings_btn_clicked), NULL);
    g_signal_connect(home_btn, "clicked", G_CALLBACK(home_btn_clicked), NULL);
}
