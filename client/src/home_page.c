#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

t_list *friend_list;
t_list *user_list;

void show_home(void)
{
    // main box
    //    current_grid.main_grid = create_grid(1200, 760, NULL);

    gtk_widget_set_hexpand(current_grid.main_grid, TRUE);
    gtk_widget_set_vexpand(current_grid.main_grid, TRUE);
    
    current_grid.intro_grid = create_grid(1200, 760, NULL);

    // help containers
    current_grid.chats_container = create_grid(1010, 667, "chats_container");
    current_grid.three_rows_container = create_grid(427, 667, NULL);

    // the children of the main grid
    current_grid.left_menu_bar = create_grid(124, 667, "left_menu_bar");
    current_grid.your_profile = create_grid(1010, 667, "home");
    current_grid.home = create_grid(1010, 667, "home");
    current_grid.search_bar = create_grid(427, 59, "search_bar_grid");
    current_grid.mini_groups = create_grid(427, 318, "mini_groups_grid");
    current_grid.mini_chats = create_grid(427, 246, "mini_chats");
    current_grid.chats = create_grid(557, 667, "chats_list_grid");
    current_grid.empty_chat = create_grid(557, 667, "empty_chat_grid");
    current_grid.chat_with_friend = create_grid(557, 667, "empty_chat_grid");
    current_grid.settings = create_grid(1010, 667, "settings_main_grid");
    current_grid.achievements = create_grid(1010, 667, "achievements_grid");

    // the children of the intro main grid
    current_grid.first_intro_screen = create_grid(1200, 760, "intro_main_box");
    current_grid.second_intro_screen = create_grid(1200, 760, "intro_flash_light");
    current_grid.third_intro_screen = create_grid(586, 544, NULL);

    cJSON *json1 = cJSON_CreateObject();
    cJSON_AddStringToObject(json1, "login", current_client.login);
    cJSON_AddStringToObject(json1, "command", "<friend_list>");

    char *json_str1 = cJSON_Print(json1);
    cJSON_Delete(json1);

    send_message_to_server(json_str1);

    while (!friend_list)
    {
        friend_list = receive_list(current_client.ssl);
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", current_client.login);
    cJSON_AddStringToObject(json, "command", "<user_list>");

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    send_message_to_server(json_str);

    while(user_list == NULL) {
        user_list = receive_list(current_client.ssl);
    }

    // fill all grids
    show_left_menu_bar();
    show_achievements();
    show_your_profile();
    show_home_grid();
    show_settings();
    show_search_bar();
    show_mini_groups();
    show_mini_chats();
    show_empty_chat();
    show_create_new_chat_with_someone();

    // fill intro grids
    //  first_intro_screen();
    //  second_intro_screen();
    //  third_intro_screen();

    // //create the structure of the intro screen
    // gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.intro_grid, 0, 0, 1, 1);
    // gtk_grid_attach(GTK_GRID(current_grid.intro_grid), current_grid.first_intro_screen, 0, 0, 1, 1);
    // gtk_grid_attach(GTK_GRID(current_grid.intro_grid), current_grid.second_intro_screen, 0, 0, 1, 1);
    // gtk_grid_attach(GTK_GRID(current_grid.intro_grid), current_grid.third_intro_screen, 0, 0, 1, 1);

    // create the structure of chat main screen
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.left_menu_bar, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.chats_container, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.chats_container), current_grid.three_rows_container, 0, 0, 1, 1);
    gtk_grid_set_row_spacing(GTK_GRID(current_grid.three_rows_container), 23);
    gtk_grid_attach(GTK_GRID(current_grid.three_rows_container), current_grid.search_bar, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.three_rows_container), current_grid.mini_groups, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.three_rows_container), current_grid.mini_chats, 0, 2, 1, 1);
    gtk_grid_set_column_spacing(GTK_GRID(current_grid.chats_container), 21);
    gtk_grid_attach(GTK_GRID(current_grid.chats_container), current_grid.chats, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.chats_container), current_grid.empty_chat, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.chats_container), current_grid.chat_with_friend, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.your_profile, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.home, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.settings, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.main_grid), current_grid.achievements, 1, 0, 1, 1);

    // gtk_widget_set_margin_start(current_grid.home, 23);
    gtk_widget_set_margin_end(current_grid.home, 21);

    // set unvisible all
    set_unvisible_all();
    // show intro
    if (current_grid.is_log_in_clicked)
    {
        // gtk_widget_set_visible(GTK_WIDGET(current_grid.intro_grid), TRUE);
        // set_first_intro_screen_visible();
        gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
        gtk_widget_set_visible(GTK_WIDGET(current_grid.home), TRUE);
    }
    else
    {
        // gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), TRUE);
        // gtk_widget_set_visible(GTK_WIDGET(current_grid.home), TRUE);
    }

    //    //by default at first show home grid
    //    gtk_widget_set_visible(GTK_WIDGET(current_grid.home), TRUE);

    // set main grid as a chlid to a widow
    gtk_window_set_child(GTK_WINDOW(current_screen.screen), current_grid.main_grid);
}

void set_unvisible_all()
{
    gtk_widget_set_visible(GTK_WIDGET(current_grid.log_in_conrainer), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.registration_container), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.intro_grid), FALSE);

    gtk_widget_set_visible(GTK_WIDGET(current_grid.left_menu_bar), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.chats_container), FALSE);

    gtk_widget_set_visible(GTK_WIDGET(current_grid.your_profile), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.home), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.settings), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(current_grid.achievements), FALSE);
}
