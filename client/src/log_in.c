#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
t_log_in current_log_in;

static void dont_have_account_btn_clicked(void)
{
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.registration_container), TRUE);
}

static int get_username_status(void)
{

    if (mx_strlen(current_client.login) == 0)
        return -3;
    if (mx_strlen(current_client.password) == 0)
        return -4;

    char *json_str;
    json_str = registration(0);
    send_message_to_server(json_str);
    mx_printstr(json_str);
    char buf[256];

    while (main_client.connected == false)
    {
        pthread_mutex_lock(&mutex_recv);
        int len = SSL_read(current_client.ssl, buf, sizeof(buf));
        pthread_mutex_unlock(&mutex_recv);
        if (len < 0)
        {
            return -1;
        }
        else if (mx_strcmp(buf, "user not found\n") == 0)
        {

            return -2;
        }
        else if (mx_strcmp(buf, "incorrect password\n") == 0)
        {

            return -5;
        }
        else if (mx_strcmp(buf, "success\n") == 0)
        {
            pthread_mutex_lock(&mutex_recv);
            main_client.connected = true;
            pthread_cond_broadcast(&auth_cond);
            pthread_mutex_unlock(&mutex_recv);
            return 0;
        }
    }

    return 0;
}

// static int get_password_status (const char *confirm_password) {

//     if (mx_strlen(current_client.password) < 4) return -1;
//     if (mx_strlen(current_client.password) > 10) return -2;

//     const char *forbidden_symbols = "$&=+<>,_`- ";

//     for (int i = 0; i < mx_strlen(current_client.password); i++) {

//         for (int j = 0; j < 9; j++) {

//             if (current_client.password[i] == forbidden_symbols[j]) {

//                 return -3;

//             }

//         }

//     }

//     if (mx_strcmp(current_client.password, confirm_password) != 0) return -4;

//     return 0;

// }

static void log_in_btn_clicked(GtkWidget *widget, gpointer data)
{
    bool is_log_in_success = true;

    current_grid.is_log_in_clicked = TRUE;

    GtkWidget **entry_data = data;
    GtkEntryBuffer *username = gtk_entry_get_buffer(GTK_ENTRY(entry_data[0]));
    GtkEntryBuffer *password = gtk_entry_get_buffer(GTK_ENTRY(entry_data[1]));

    current_client.login = mx_strdup(gtk_entry_buffer_get_text(username));
    current_client.password = mx_strdup(gtk_entry_buffer_get_text(password));

    int username_status = 0;
    if (!main_client.connected)
    {
        username_status = get_username_status();
    }
    switch (username_status)
    {

    case 0:
    {

        widget_restyling(current_log_in.username_error_label, current_screen, "error_label", "hide_label");

        widget_restyling(entry_data[0], current_screen, "wrong_auth_entry_field", "auth_entry_field");

        break;
    }

    case -1:
    {

        gtk_label_set_text(GTK_LABEL(current_log_in.username_error_label), "Unable to receive data from server!");

        widget_restyling(current_log_in.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_log_in_success = false;

        return;
    }

    case -2:
    {

        gtk_label_set_text(GTK_LABEL(current_log_in.username_error_label), "Incorrect username or password!");

        widget_restyling(current_log_in.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_log_in_success = false;

        return;
    }

    case -3:
    {

        gtk_label_set_text(GTK_LABEL(current_log_in.username_error_label), "Please enter username!");

        widget_restyling(current_log_in.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_log_in_success = false;

        return;
    }

    case -4:
    {

        gtk_label_set_text(GTK_LABEL(current_log_in.password_error_label), "Please enter password!");

        widget_restyling(current_log_in.password_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");

        is_log_in_success = false;

        return;
    }

    case -5:
    {

        gtk_label_set_text(GTK_LABEL(current_log_in.password_error_label), "incorrect password!");

        widget_restyling(current_log_in.password_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");

        is_log_in_success = false;

        return;
    }
    }

    if (is_log_in_success)
    {
        main_client.connected = true;
        set_unvisible_auth();
        show_home();
    }
}

void show_log_in(void)
{

    current_log_in.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    current_log_in.welcome = gtk_label_new_with_mnemonic("Welcome to the dark!");

    current_log_in.log_in_button = gtk_button_new_with_mnemonic("Log in");

    current_log_in.dont_have_account = gtk_label_new_with_mnemonic("Don’t have an account?");

    current_log_in.sign_up_button_log_in = gtk_button_new_with_label("Sign up");

    gtk_widget_set_halign(current_log_in.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(current_log_in.box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(current_log_in.box, TRUE);
    gtk_widget_set_vexpand(current_log_in.box, TRUE);

    // PangoFontDescription *font_desc = pango_font_description_from_string("MyFont 12");

    // gtk_widget_override_font(current_log_in.welcome, font_desc);
    // set_font(current_log_in.welcome, "../../resources/fonts/JetBrains_Mono/static/JetBrainsMono-Regular.ttf");

    gtk_grid_attach(GTK_GRID(current_grid.log_in_conrainer), current_log_in.box, 0, 0, 1, 1);

    gtk_widget_set_margin_start(current_log_in.box, 307);
    gtk_widget_set_margin_end(current_log_in.box, 307);
    gtk_widget_set_margin_top(current_log_in.box, 108);
    gtk_widget_set_margin_bottom(current_log_in.box, 108);

    gtk_widget_set_size_request(current_log_in.box, 586, 544);

    current_log_in.username = gtk_entry_new();
    current_log_in.username_error_label = gtk_label_new("label");

    current_log_in.password = gtk_entry_new();
    current_log_in.password_error_label = gtk_label_new("label");

    gtk_entry_set_alignment(GTK_ENTRY(current_log_in.username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(current_log_in.password), 0.1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_log_in.username), " Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_log_in.password), " Password");
    gtk_entry_set_visibility(GTK_ENTRY(current_log_in.password), FALSE);

    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.welcome);

    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.username);
    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.username_error_label);

    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.password);
    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.password_error_label);

    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.log_in_button);
    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.dont_have_account);
    gtk_box_append(GTK_BOX(current_log_in.box), current_log_in.sign_up_button_log_in);

    gtk_widget_set_halign(current_log_in.welcome, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.welcome, 60);

    gtk_widget_set_halign(current_log_in.username, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.username, 48);
    gtk_widget_set_size_request(current_log_in.username, 423, 63);

    gtk_widget_set_halign(current_log_in.username_error_label, GTK_ALIGN_CENTER);

    gtk_widget_set_halign(current_log_in.password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.password, 5);
    gtk_widget_set_size_request(current_log_in.password, 423, 63);

    gtk_widget_set_halign(current_log_in.password_error_label, GTK_ALIGN_CENTER);

    gtk_widget_set_halign(current_log_in.log_in_button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.log_in_button, 17);
    gtk_widget_set_size_request(current_log_in.log_in_button, 423, 53);

    gtk_widget_set_halign(current_log_in.dont_have_account, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.dont_have_account, 25);
    gtk_widget_set_size_request(current_log_in.dont_have_account, 260, 23);

    gtk_widget_set_halign(current_log_in.sign_up_button_log_in, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_log_in.sign_up_button_log_in, 0);
    gtk_widget_set_size_request(current_log_in.sign_up_button_log_in, 63, 20);

    widget_styling(current_log_in.box, current_screen, "auth_main_box");
    widget_styling(current_log_in.welcome, current_screen, "auth_welcome_to_the_dark");
    widget_styling(current_log_in.username, current_screen, "auth_entry_field");
    widget_styling(current_log_in.username_error_label, current_screen, "hide_label");
    widget_styling(current_log_in.password, current_screen, "auth_entry_field");
    widget_styling(current_log_in.password_error_label, current_screen, "hide_label");
    widget_styling(current_log_in.log_in_button, current_screen, "auth_button");
    widget_styling(current_log_in.dont_have_account, current_screen, "auth_dont_or_have_account");
    widget_styling(current_log_in.sign_up_button_log_in, current_screen, "auth_sign_button");

    GtkWidget **entry_arr = (GtkWidget **)malloc(2 * sizeof(GtkWidget *)); //{LOGIN_entry_field1, LOGIN_entry_field2};
    entry_arr[0] = current_log_in.username;
    entry_arr[1] = current_log_in.password;

    g_signal_connect(current_log_in.sign_up_button_log_in, "clicked", G_CALLBACK(dont_have_account_btn_clicked), NULL);

    g_signal_connect(current_log_in.log_in_button, "clicked", G_CALLBACK(log_in_btn_clicked), entry_arr);
}
