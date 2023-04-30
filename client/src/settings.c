#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

void change_scheme_to_any_color(char *bg_win_col,
                                char *bg_auth_main_box,
                                char *bit_text_col,
                                char *small_text_col,
                                char *auth_input_bg_col,
                                char *auth_input_border_col,
                                char *auth_input_border_col_hover,
                                char *big_log_in_button_bg,
                                char *big_log_in_button_shadow_hover,
                                char *big_log_in_button_shadow_active,
                                char *sign_btn_text_col,
                                char *sign_btn_text_col_hover,
                                char *all_containers_bg,
                                char *right_chat_container_bg,
                                char *all_containers_borders_bg,
                                char *left_menu_bar_icon_background,
                                char *left_menu_bar_icon_hover,
                                char *chat_and_group_label_text_color,
                                char *log_out_container_bg,
                                char *log_out_yes_border,
                                char *log_out_no_border) {
    FILE *css_file = fopen("resources/style/color.css", "w");
    char our_final_line[2024];
    sprintf(our_final_line, "/*AUTH START*/\n"
                            "/*window background*/\n"
                            "@define-color bg-win-col %s;\n"
                            "\n"
                            "/*auth main box background*/\n"
                            "@define-color bg-auth-main-box %s;\n"
                            "\n"
                            "/*all text*/\n"
                            "@define-color bit-text-col %s;\n"
                            "\n"
                            "/*text in achievements*/\n"
                            "@define-color small-text-col %s;\n"
                            "\n"
                            "/*auth inputs background*/\n"
                            "@define-color auth-input-bg-col %s;\n"
                            "\n"
                            "/*auth inputs border color*/\n"
                            "@define-color auth-input-border-col %s;\n"
                            "\n"
                            "/*auth inputs border color hover*/\n"
                            "@define-color auth-input-border-col-hover %s;\n"
                            "\n"
                            "/*big log in button*/\n"
                            "@define-color big-log-in-button-bg %s;\n"
                            "\n"
                            "/*big log in button hover shadow*/\n"
                            "@define-color big-log-in-button-shadow-hover %s;\n"
                            "\n"
                            "/*big log in button active shadow*/\n"
                            "@define-color big-log-in-button-shadow-active %s;\n"
                            "\n"
                            "/*sign button text color*/\n"
                            "@define-color sign-btn-text-col %s;\n"
                            "\n"
                            "/*sign button text color*/\n"
                            "@define-color sign-btn-text-col-hover %s;\n"
                            "\n"
                            "/*AUTH END*/\n"
                            "\n"
                            "/*all containers bg*/\n"
                            "@define-color all-containers-bg %s;\n"
                            "\n"
                            "/*right chat container bg*/\n"
                            "@define-color right-chat-container-bg %s;\n"
                            "\n"
                            "/*all containers borders in rgba*/\n"
                            "@define-color all-containers-borders-bg %s;\n"
                            "\n"
                            "/*left menu bar icon background*/\n"
                            "@define-color left-menu-bar-icon-background %s;\n"
                            "\n"
                            "/*left menu bar icon hovers*/\n"
                            "@define-color left-menu-bar-icon-hover %s;\n"
                            "\n"
                            "/*chat and group label text color*/\n"
                            "@define-color chat-and-group-label-text-color %s;\n"
                            "\n"
                            "/*log out container bg*/\n"
                            "@define-color log-out-container-bg %s;\n"
                            "\n"
                            "/*log out yes border*/\n"
                            "@define-color log-out-yes-border %s;\n"
                            "\n"
                            "/*log out no border*/\n"
                            "@define-color log-out-no-border %s;\n"
                            "", bg_win_col, bg_auth_main_box, bit_text_col, small_text_col,
            auth_input_bg_col, auth_input_border_col, auth_input_border_col_hover,
            big_log_in_button_bg, big_log_in_button_shadow_hover,
            big_log_in_button_shadow_active, sign_btn_text_col, sign_btn_text_col_hover,
            all_containers_bg, right_chat_container_bg,
            all_containers_borders_bg, left_menu_bar_icon_background, left_menu_bar_icon_hover,
            chat_and_group_label_text_color, log_out_container_bg, log_out_yes_border, log_out_no_border);

//    strcat(our_final_line, color);
//    strcat(our_final_line, ";");

    fprintf(css_file, "%s\n", our_final_line);
    fclose(css_file);
}

void settings_first_scheme_btn_click() {
    change_scheme_to_any_color("#171722",
                               "#212130",
                               "#B1BAE6",
                               "#7AA2F7",
                               "#434461",
                               "#434461",
                               "#6E78A8",
                               "#13131A",
                               "#565F89",
                               "#565F89",
                               "#565F89",
                               "#0269A4",
                               "#13131A",
                               "#171722",
                               "rgba(122, 162, 247, 0.35)",
                               "#4A4A66",
                               "#7AA2F7",
                               "rgba(122, 162, 247, 0.5)",
                               "#212130",
                               "#434461",
                               "#13131A");
    gtk_css_provider_load_from_path(current_screen.provider,
                                    "resources/style/theme.css");
}

void settings_second_scheme_btn_click() {
    change_scheme_to_any_color("#383a59",
                               "#21222c",
                               "#ffffff",
                               "#bd93f9",
                               "#282a36",
                               "#6272a4",
                               "#bd93f9",
                               "#6272a4",
                               "#bd93f9",
                               "#bd93f9",
                               "#565F89",
                               "#8be9fd",
                               "#383a59",
                               "#50527f",
                               "#bd93f9",
                               "#44475a",
                               "#575a72",
                               "#bd93f9",
                               "#383a59",
                               "#bd93f9",
                               "#5e343d");
    gtk_css_provider_load_from_path(current_screen.provider,
                                    "resources/style/theme.css");
}

void settings_third_scheme_btn_click() {
    change_scheme_to_any_color("green",
                               "#212130",
                               "#B1BAE6",
                               "#7AA2F7",
                               "#434461",
                               "#434461",
                               "#6E78A8",
                               "#13131A",
                               "#565F89",
                               "#565F89",
                               "#565F89",
                               "#0269A4",
                               "#13131A",
                               "#171722",
                               "rgba(122, 162, 247, 0.35)",
                               "#4A4A66",
                               "#7AA2F7",
                               "rgba(122, 162, 247, 0.5)",
                               "#212130",
                               "#434461",
                               "#13131A");
    gtk_css_provider_load_from_path(current_screen.provider,
                                    "resources/style/theme.css");
}

GtkWidget *create_color_circle(const gchar *const style) {
    GtkWidget *circle = gtk_button_new();
    gtk_widget_set_size_request(circle, 35, 35);

    widget_styling(circle, current_screen, style);

    return circle;
}

void show_settings() {

    GtkWidget *settings_btn = gtk_button_new();

    GtkWidget *settings_label_btn = gtk_button_new_with_label("Settings"); ////

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    GtkWidget *pick_theme_label = gtk_label_new("Pick your theme!");
    
    GtkWidget *choose_theme_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);


    GtkWidget *settings_first_scheme_btn = create_color_circle("settings_default_scheme_btn");
    GtkWidget *settings_second_scheme_btn = create_color_circle("settings_purple_scheme_btn");
    GtkWidget *settings_third_scheme_btn = create_color_circle("settings_dark_scheme_btn");


    gtk_widget_set_hexpand(settings_btn, TRUE);
    gtk_widget_set_halign(settings_btn, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_top(settings_btn, 30);

    gtk_widget_set_size_request(settings_btn, 200, 200);


    gtk_widget_set_hexpand(settings_label_btn, TRUE);
    gtk_widget_set_halign(settings_label_btn, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_top(settings_label_btn, 14);

    gtk_widget_set_size_request(settings_label_btn, 160, 35);

    
    gtk_grid_attach(GTK_GRID(current_grid.settings), settings_btn, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.settings), settings_label_btn, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(current_grid.settings), box, 0, 2, 1, 1);

    gtk_box_append(GTK_BOX(box), pick_theme_label);

    gtk_box_append(GTK_BOX(box), choose_theme_box);

    gtk_box_append(GTK_BOX(choose_theme_box), settings_first_scheme_btn);

    gtk_box_append(GTK_BOX(choose_theme_box), settings_second_scheme_btn);

    gtk_box_append(GTK_BOX(choose_theme_box), settings_third_scheme_btn);

    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_top(box, 22);
    gtk_widget_set_margin_bottom(box, 18);

    gtk_widget_set_size_request(box, 474, 347);

    gtk_widget_set_hexpand(pick_theme_label, TRUE);
    gtk_widget_set_vexpand(pick_theme_label, TRUE);
    gtk_widget_set_halign(pick_theme_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(pick_theme_label, GTK_ALIGN_START);

    gtk_widget_set_margin_top(pick_theme_label, 20);

    gtk_widget_set_size_request(pick_theme_label, 133, 24);

    gtk_widget_set_hexpand(choose_theme_box, TRUE);
    //gtk_widget_set_vexpand(choose_theme_box, TRUE);
    gtk_widget_set_halign(choose_theme_box, GTK_ALIGN_CENTER);
    //gtk_widget_set_valign(choose_theme_box, GTK_ALIGN_START);

    gtk_widget_set_margin_top(choose_theme_box, 20);
    gtk_widget_set_margin_bottom(choose_theme_box, 220);

    gtk_widget_set_size_request(choose_theme_box, 250, 60);
    

    gtk_widget_set_hexpand(settings_first_scheme_btn, TRUE);
    gtk_widget_set_vexpand(settings_first_scheme_btn, TRUE);
    gtk_widget_set_halign(settings_first_scheme_btn, GTK_ALIGN_START);
    gtk_widget_set_valign(settings_first_scheme_btn, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(settings_first_scheme_btn, 10);

    gtk_widget_set_size_request(settings_first_scheme_btn, 30, 30);

    gtk_widget_set_hexpand(settings_second_scheme_btn, TRUE);
    gtk_widget_set_vexpand(settings_second_scheme_btn, TRUE);
    gtk_widget_set_halign(settings_second_scheme_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(settings_second_scheme_btn, GTK_ALIGN_CENTER);

    //gtk_widget_set_margin_top(settings_second_scheme_btn, 20);

    gtk_widget_set_size_request(settings_second_scheme_btn, 30, 30);

    gtk_widget_set_hexpand(settings_third_scheme_btn, TRUE);
    gtk_widget_set_vexpand(settings_third_scheme_btn, TRUE);
    gtk_widget_set_halign(settings_third_scheme_btn, GTK_ALIGN_END);
    gtk_widget_set_valign(settings_third_scheme_btn, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_end(settings_third_scheme_btn, 10);

    gtk_widget_set_size_request(settings_third_scheme_btn, 30, 30);



    widget_styling(settings_btn, current_screen, "settings_icon_btn");

    widget_styling(settings_label_btn, current_screen, "settings_label_btn");

    widget_styling(box, current_screen, "choose_theme_box");

    widget_styling(choose_theme_box, current_screen, "choose_theme_box");

    widget_styling(pick_theme_label, current_screen, "pick_theme_label");


    g_signal_connect(settings_first_scheme_btn, "clicked", G_CALLBACK(settings_first_scheme_btn_click), NULL);
    g_signal_connect(settings_second_scheme_btn, "clicked", G_CALLBACK(settings_second_scheme_btn_click), NULL);
    g_signal_connect(settings_third_scheme_btn, "clicked", G_CALLBACK(settings_third_scheme_btn_click), NULL);
}

