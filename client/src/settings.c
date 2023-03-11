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

    GtkWidget *settings_grid_help = create_grid(470, 629, "settings_grid_help");
    GtkWidget *settings_grid_for_ico = create_grid(201, 201, "settings_grid_for_ico");
    GtkWidget *settings_grid_with_ico = create_grid(200, 200, "settings_grid_with_ico");
    GtkWidget *settings_grid_with_lab = create_grid(160, 35, "settings_grid_with_lab");
    GtkWidget *settings_grid_with_body = create_grid(470, 340, "settings_grid_with_body");
    GtkWidget *settings_grid_with_color_scheme = create_grid(190, 85, "settings_grid_with_in_body");
    GtkWidget *settings_grid_with_chp = create_grid(190, 55, NULL);
    GtkWidget *settings_grid_with_da = create_grid(190, 55, NULL);
    GtkWidget *settings_grid_with_chp_for_btn = create_grid(190, 55, "settings_grid_with_in_body");
    GtkWidget *settings_grid_with_da_for_btn = create_grid(190, 55, "settings_grid_with_in_body");
    GtkWidget *settings_grid_with_chp_ico = create_grid(35, 35, NULL);
    GtkWidget *settings_grid_with_da_ico = create_grid(35, 35, NULL);
    GtkWidget *settings_grid_with_chp_lab = create_grid(120, 55, NULL);
    GtkWidget *settings_grid_with_da_lab = create_grid(120, 55, NULL);
    GtkWidget *settings_grid_for_new_lab = create_grid(190, 35, NULL);
    GtkWidget *settings_grid_with_color_scheme_list = create_grid(190, 55, NULL);

    GtkWidget *color_scheme_grid_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(color_scheme_grid_scrolled), settings_grid_with_color_scheme_list);
    gtk_widget_set_size_request(color_scheme_grid_scrolled, 203, 55);

    GtkWidget *settings_lab = gtk_label_new("Settings");
    GtkWidget *settings_lab_nsh = gtk_label_new("Choose your theme");
    GtkWidget *settings_lab_chp = gtk_label_new("Change password");
    GtkWidget *settings_lab_da = gtk_label_new("Delete account");
    GtkWidget *settings_button_chp = gtk_button_new();
    GtkWidget *settings_button_da = gtk_button_new();

    gtk_widget_set_halign(settings_lab, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(settings_lab_nsh, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(settings_lab_chp, GTK_ALIGN_CENTER);

    gtk_widget_set_size_request(settings_button_chp, 190, 55);
    gtk_widget_set_size_request(settings_button_da, 190, 55);

    GtkWidget *settings_first_scheme_btn = create_color_circle("settings_default_scheme_btn");
    GtkWidget *settings_second_scheme_btn = create_color_circle("settings_purple_scheme_btn");
    GtkWidget *settings_third_scheme_btn = create_color_circle("settings_dark_scheme_btn");

    gtk_grid_attach(GTK_GRID(current_grid.settings), settings_grid_help, 0, 0, 1, 1);
    gtk_grid_set_row_spacing(GTK_GRID(settings_grid_help), 19);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_for_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_for_ico), settings_grid_with_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_with_lab, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_help), settings_grid_with_body, 0, 2, 1, 1);

    gtk_widget_set_size_request(settings_lab_nsh, 180, 30);

    widget_styling(settings_lab, current_screen, "settings_lab");
    widget_styling(settings_lab_nsh, current_screen, "settings_lab_in_body");
    widget_styling(settings_lab_chp, current_screen, "settings_lab_in_body");
    widget_styling(settings_lab_da, current_screen, "settings_lab_in_body");
    widget_styling(settings_button_chp, current_screen, "settings_grid_with_in_body_btn");
    widget_styling(settings_button_da, current_screen, "settings_grid_with_in_body_btn");

    gtk_grid_attach(GTK_GRID(settings_grid_with_lab), settings_lab, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_color_scheme, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), settings_grid_for_new_lab, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme), color_scheme_grid_scrolled, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_for_new_lab), settings_lab_nsh, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_chp_for_btn, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_chp_for_btn), settings_button_chp, 0, 1, 1, 1);
    gtk_button_set_child(GTK_BUTTON(settings_button_chp), settings_grid_with_chp);
    gtk_grid_attach(GTK_GRID(settings_grid_with_chp), settings_grid_with_chp_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_chp), settings_grid_with_chp_lab, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_chp_lab), settings_lab_chp, 0, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(settings_grid_with_body), settings_grid_with_da_for_btn, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_da_for_btn), settings_button_da, 0, 1, 1, 1);
    gtk_button_set_child(GTK_BUTTON(settings_button_da), settings_grid_with_da);
    gtk_grid_attach(GTK_GRID(settings_grid_with_da), settings_grid_with_da_ico, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_da), settings_grid_with_da_lab, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_da_lab), settings_lab_da, 0, 0, 1, 1);


    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_first_scheme_btn, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_second_scheme_btn, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(settings_grid_with_color_scheme_list), settings_third_scheme_btn, 2, 0, 1, 1);

    g_signal_connect(settings_first_scheme_btn, "clicked", G_CALLBACK(settings_first_scheme_btn_click), NULL);
    g_signal_connect(settings_second_scheme_btn, "clicked", G_CALLBACK(settings_second_scheme_btn_click), NULL);
    g_signal_connect(settings_third_scheme_btn, "clicked", G_CALLBACK(settings_third_scheme_btn_click), NULL);
}

