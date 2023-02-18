#include "../inc/client.h"

extern t_screen curent_screen;
t_grid curent_grid;

void show_home(void) {
    //main box
    curent_grid.main_grid = create_grid(1200, 760, "main_grid");

    //help containers
    curent_grid.chats_container = create_grid(1010, 667, "chats_container");
    curent_grid.three_rows_container = create_grid(427, 667, NULL);

    //the children of the main grid
    curent_grid.left_menu_bar = create_grid(124, 667, "left_menu_bar");
    curent_grid.your_profile = create_grid(1010, 667, "home");
    curent_grid.home = create_grid(1010, 667, "home");
    curent_grid.search_bar = create_grid(427, 59, "search_bar_box");
    curent_grid.mini_groups = create_grid(427, 318, "mini_groups");
    curent_grid.mini_chats = create_grid(427, 246, "main_box_style");
    curent_grid.chats = create_grid(557, 667, "chats");
    curent_grid.settings = create_grid(1010, 667, "settings_main_grid");
    curent_grid.achievements = create_grid(1010, 667, "home");

    //fill all grids
    show_left_menu_bar();
    show_chats();
    show_achievements();
    show_your_profile();
    show_home_grid();
    show_settings();
    show_search_bar();
    show_mini_groups();
    show_mini_chats();

    //create the structure of chat main screen
    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.left_menu_bar, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.chats_container, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.chats_container), curent_grid.three_rows_container, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.three_rows_container), curent_grid.search_bar, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.three_rows_container), curent_grid.mini_groups, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.three_rows_container), curent_grid.mini_chats, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.chats_container), curent_grid.chats, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.your_profile, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.home, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.settings, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(curent_grid.main_grid), curent_grid.achievements, 1, 0, 1, 1);

    //set unvisible all
    set_unvisible_all();

    //by default at first show home grid
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.home), TRUE);

    //set main grid as a chlid to a widow
    gtk_window_set_child(GTK_WINDOW(curent_screen.screen), curent_grid.main_grid);
}

void set_unvisible_all() {
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.chats_container), FALSE);

    gtk_widget_set_visible(GTK_WIDGET(curent_grid.your_profile), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.home), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.settings), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.achievements), FALSE);
}
