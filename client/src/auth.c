#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

void show_auth() {
    //main auth
    current_grid.main_grid = create_grid(586, 544, NULL);

    //the children of the auth main grid
    current_grid.log_in_conrainer = create_grid(586, 544, NULL);
    current_grid.registration_container = create_grid(586, 544, NULL);
    current_grid.registration_success_container = create_grid(586, 544, NULL);

    //fill auth grids
    show_log_in();
    show_registration();
    show_success_registration();


    //create the structure of the auth screen
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.log_in_conrainer, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.registration_container, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.registration_success_container, 0, 0, 1, 1);

    //set unvisible auth
    set_unvisible_auth();

    //by default at first show log in grid
    gtk_widget_set_visible(GTK_WIDGET(current_grid.log_in_conrainer), TRUE);

    //set main grid as a chlid to a widow
    gtk_window_set_child(GTK_WINDOW(current_screen.screen), current_grid.main_grid);

}

void set_unvisible_auth() {
    
    gtk_widget_set_visible(GTK_WIDGET(current_grid.log_in_conrainer), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.registration_container), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.registration_success_container), FALSE);

}
