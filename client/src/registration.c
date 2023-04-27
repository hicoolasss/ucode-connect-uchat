#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
extern t_client current_client;
t_registration current_registration;

static void log_in_btn_clicked(void)
{

    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.log_in_conrainer), TRUE);
    show_log_in();
}

static int get_username_status(void)
{

    if (mx_strlen(current_client.login) < 4)
        return -1;
    if (mx_strlen(current_client.login) > 10)
        return -2;

    const char *forbidden_symbols = "$&=+<>,_`- ";

    for (int i = 0; i < mx_strlen(current_client.login); i++)
    {

        for (int j = 0; j < 9; j++)
        {

            if (current_client.login[i] == forbidden_symbols[j])
            {

                return -3;
            }
        }
    }

    char *json_str;
    json_str = registration(1);
    send_message_to_server(json_str);
    char buf[256];

    while (1)
    {
        // int len = recv_all(current_client.ssl, buf, sizeof(buf));
        printf("Thread %lu trying to lock mutex\n", pthread_self());
        pthread_mutex_lock(&mutex_recv);
        int len = SSL_read(current_client.ssl, buf, sizeof(buf));
        pthread_mutex_unlock(&mutex_recv);
        printf("Thread %lu trying to unlock mutex\n", pthread_self());
        if (len < 0)
        {

            // printf("Error: Unable to receive data from server\n");
            return -4;
        }
        else if (mx_strcmp(buf, "this user already exist\n") == 0)
        {

            // mx_printstr("this user already exist\n");
            return -5;
        }
        else if (mx_strcmp(buf, "registered\n") == 0)
        {

            return 0;
        }
        else break;
    }
    return 1;
}
static int get_password_status(const char *confirm_password)
{

    if (mx_strlen(current_client.password) < 4)
        return -1;
    if (mx_strlen(current_client.password) > 10)
        return -2;

    const char *forbidden_symbols = "$&=+<>,_`- ";

    for (int i = 0; i < mx_strlen(current_client.password); i++)
    {

        for (int j = 0; j < 9; j++)
        {

            if (current_client.password[i] == forbidden_symbols[j])
            {

                return -3;
            }
        }
    }

    if (mx_strcmp(current_client.password, confirm_password) != 0)
        return -4;

    return 0;
}

static void sign_inbtn_clicked(GtkWidget *btn)
{
    (void)btn;
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(current_grid.log_in_conrainer), TRUE);
}

static void username_to_password(GtkEntry *entry)
{
    // gtk_entry_set_text(entry, "");
    (void)entry;
    gtk_widget_grab_focus(current_registration.password); // Та-да! Курсор перемещается на второй виджет GtkEntry
}

static void password_to_confirm_password(GtkEntry *entry)
{
    // gtk_entry_set_text(entry, "");
    (void)entry;
    gtk_widget_grab_focus(current_registration.confirm_password); // Та-да! Курсор перемещается на второй виджет GtkEntry
}

static void sign_up_btn_clicked(GtkWidget *widget, gpointer entries_array)
{

    (void)widget;

    bool is_registration_success = true;

    GtkWidget **entry_data = entries_array;

    GtkEntryBuffer *username = gtk_entry_get_buffer(GTK_ENTRY(entry_data[0]));
    GtkEntryBuffer *password1 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[1]));
    GtkEntryBuffer *password2 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[2]));

    current_client.login = mx_strdup(gtk_entry_buffer_get_text(username));
    current_client.password = mx_strdup(gtk_entry_buffer_get_text(password1));
    const char *confirm_password = mx_strdup(gtk_entry_buffer_get_text(password2));

    int username_status = get_username_status();

    switch (username_status)
    {

    case 0:
    {

        widget_restyling(current_registration.username_error_label, current_screen, "error_label", "hide_label");

        widget_restyling(entry_data[0], current_screen, "wrong_auth_entry_field", "auth_entry_field");

        break;
    }

    case -1:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.username_error_label), "Username is too short!");

        widget_restyling(current_registration.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -2:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.username_error_label), "Username is too long!");

        widget_restyling(current_registration.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -3:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.username_error_label), "Username contain forbidden symbols!");

        widget_restyling(current_registration.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -4:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.username_error_label), "Unable to receive data from server!");

        widget_restyling(current_registration.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -5:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.username_error_label), "This account already exist!");

        widget_restyling(current_registration.username_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[0], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }
    }

    int password_status = get_password_status(confirm_password);

    switch (password_status)
    {

    case 0:
    {

        widget_restyling(current_registration.password_error_label, current_screen, "error_label", "hide_label");
        widget_restyling(current_registration.confirm_password_error_label, current_screen, "error_label", "hide_label");

        widget_restyling(entry_data[1], current_screen, "wrong_auth_entry_field", "auth_entry_field");
        widget_restyling(entry_data[2], current_screen, "wrong_auth_entry_field", "auth_entry_field");

        break;
    }

    case -1:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.password_error_label), "Password is too short");

        widget_restyling(current_registration.password_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -2:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.password_error_label), "Password is too long");

        widget_restyling(current_registration.password_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -3:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.password_error_label), "Password contain forbidden symbols!");

        widget_restyling(current_registration.password_error_label, current_screen, "hide_label", "error_label");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");

        is_registration_success = false;

        return;
    }

    case -4:
    {

        gtk_label_set_text(GTK_LABEL(current_registration.password_error_label), "Passwords missmatch!");
        gtk_label_set_text(GTK_LABEL(current_registration.confirm_password_error_label), "Passwords missmatch!");

        widget_styling(entry_data[1], current_screen, "wrong_auth_entry_field");
        widget_styling(entry_data[2], current_screen, "wrong_auth_entry_field");

        widget_restyling(current_registration.password_error_label, current_screen, "hide_label", "error_label");
        widget_restyling(current_registration.confirm_password_error_label, current_screen, "hide_label", "error_label");

        is_registration_success = false;

        return;
    }
    }

    if (is_registration_success)
    {

        gtk_editable_set_text(GTK_EDITABLE(current_registration.username), "");
        gtk_editable_set_text(GTK_EDITABLE(current_registration.password), "");
        gtk_editable_set_text(GTK_EDITABLE(current_registration.confirm_password), "");
        set_unvisible_auth();
        gtk_widget_set_visible(GTK_WIDGET(current_grid.registration_success_container), TRUE);
    }
    // pthread_mutex_unlock(&mutex1);
}

void show_registration()
{

    // GtkWidget *box, *username, *wrong_username, *password, *wrong_password, *confirm_password, *wrong_password1;
    current_registration.username_error_label = gtk_label_new("label");

    current_registration.password_error_label = gtk_label_new("label");

    current_registration.confirm_password_error_label = gtk_label_new("label");

    current_registration.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    current_registration.welcome = gtk_label_new_with_mnemonic("Welcome to the TokyoNight!");

    current_registration.sign_up_btn = gtk_button_new_with_mnemonic("Sign up");

    current_registration.have_account = gtk_label_new_with_mnemonic("Have account?");

    current_registration.sign_in_btn = gtk_button_new_with_label("Sign in");

    gtk_widget_set_halign(current_registration.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(current_registration.box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(current_registration.box, TRUE);
    gtk_widget_set_vexpand(current_registration.box, TRUE);

    gtk_grid_attach(GTK_GRID(current_grid.registration_container), current_registration.box, 0, 0, 1, 1);

    gtk_widget_set_margin_start(current_registration.box, 307);
    gtk_widget_set_margin_end(current_registration.box, 307);
    gtk_widget_set_margin_top(current_registration.box, 108);
    gtk_widget_set_margin_bottom(current_registration.box, 108);

    gtk_widget_set_size_request(current_registration.box, 586, 544);

    current_registration.username = gtk_entry_new();
    current_registration.password = gtk_entry_new();
    current_registration.confirm_password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(current_registration.username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(current_registration.password), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(current_registration.confirm_password), 0.1);

    gtk_entry_set_placeholder_text(GTK_ENTRY(current_registration.username), " Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_registration.password), " Password");
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_registration.confirm_password), "  Confirm Password");

    gtk_box_append(GTK_BOX(current_registration.box), current_registration.welcome);

    gtk_box_append(GTK_BOX(current_registration.box), current_registration.username);
    gtk_box_append(GTK_BOX(current_registration.box), current_registration.username_error_label);

    gtk_box_append(GTK_BOX(current_registration.box), current_registration.password);
    gtk_box_append(GTK_BOX(current_registration.box), current_registration.password_error_label);

    gtk_box_append(GTK_BOX(current_registration.box), current_registration.confirm_password);
    gtk_box_append(GTK_BOX(current_registration.box), current_registration.confirm_password_error_label);

    gtk_box_append(GTK_BOX(current_registration.box), current_registration.sign_up_btn);
    gtk_box_append(GTK_BOX(current_registration.box), current_registration.have_account);
    gtk_box_append(GTK_BOX(current_registration.box), current_registration.sign_in_btn);

    gtk_widget_set_halign(current_registration.welcome, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.welcome, 60);

    gtk_widget_set_halign(current_registration.username, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.username, 48);
    gtk_widget_set_size_request(current_registration.username, 423, 63);

    gtk_widget_set_halign(current_registration.username_error_label, GTK_ALIGN_CENTER);

    gtk_widget_set_halign(current_registration.password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.password, 5);
    gtk_widget_set_size_request(current_registration.password, 423, 63);

    gtk_widget_set_halign(current_registration.password_error_label, GTK_ALIGN_CENTER);

    gtk_widget_set_halign(current_registration.confirm_password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.confirm_password, 5);
    gtk_widget_set_size_request(current_registration.confirm_password, 423, 63);

    gtk_widget_set_halign(current_registration.confirm_password_error_label, GTK_ALIGN_CENTER);

    gtk_widget_set_halign(current_registration.sign_up_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.sign_up_btn, 5);
    gtk_widget_set_size_request(current_registration.sign_up_btn, 423, 53);

    gtk_widget_set_halign(current_registration.have_account, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.have_account, 12);
    gtk_widget_set_size_request(current_registration.have_account, 260, 23);

    gtk_widget_set_halign(current_registration.sign_in_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_registration.sign_in_btn, 0);
    gtk_widget_set_size_request(current_registration.sign_in_btn, 63, 20);

    widget_styling(current_registration.box, current_screen, "auth_main_box");
    widget_styling(current_registration.welcome, current_screen, "auth_welcome_to_the_dark");
    widget_styling(current_registration.username, current_screen, "auth_entry_field");
    widget_styling(current_registration.password, current_screen, "auth_entry_field");
    widget_styling(current_registration.confirm_password, current_screen, "auth_entry_field");
    widget_styling(current_registration.sign_up_btn, current_screen, "auth_button");
    widget_styling(current_registration.sign_in_btn, current_screen, "auth_sign_button");
    widget_styling(current_registration.have_account, current_screen, "auth_dont_or_have_account");

    widget_styling(current_registration.username_error_label, current_screen, "hide_label");
    widget_styling(current_registration.password_error_label, current_screen, "hide_label");
    widget_styling(current_registration.confirm_password_error_label, current_screen, "hide_label");

    g_signal_connect(current_registration.sign_in_btn, "clicked", G_CALLBACK(sign_inbtn_clicked), NULL);

    GtkWidget **entry_arr = (GtkWidget **)malloc(3 * sizeof(GtkWidget *)); //нахуй оно тут надо я не знаю но хай будет
    entry_arr[0] = current_registration.username;
    entry_arr[1] = current_registration.password;
    entry_arr[2] = current_registration.confirm_password;

    g_signal_connect(current_registration.sign_up_btn, "clicked", G_CALLBACK(sign_up_btn_clicked), entry_arr);

    g_signal_connect(current_registration.username, "activate", G_CALLBACK(username_to_password), NULL);

    g_signal_connect(current_registration.password, "activate", G_CALLBACK(password_to_confirm_password), NULL);

    g_signal_connect(current_registration.password, "activate", G_CALLBACK(sign_up_btn_clicked), NULL);

    gtk_window_present(GTK_WINDOW(current_screen.screen));

    free(entry_arr);
}

void show_success_registration(void)
{

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *registration_success_label = gtk_label_new("Registration completed successfully!");

    GtkWidget *log_in_button = gtk_button_new_with_mnemonic("Log in");

    gtk_grid_attach(GTK_GRID(current_grid.registration_success_container), box, 0, 0, 1, 1);

    gtk_widget_set_margin_start(box, 307);
    gtk_widget_set_margin_end(box, 307);
    gtk_widget_set_margin_top(box, 108);
    gtk_widget_set_margin_bottom(box, 108);

    gtk_widget_set_size_request(box, 586, 544);

    gtk_box_append(GTK_BOX(box), registration_success_label);
    gtk_box_append(GTK_BOX(box), log_in_button);

    gtk_widget_set_halign(registration_success_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(registration_success_label, 155);

    gtk_label_set_wrap(GTK_LABEL(registration_success_label), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(registration_success_label), 22);
    gtk_label_set_justify(GTK_LABEL(registration_success_label), GTK_JUSTIFY_CENTER);

    gtk_widget_set_halign(log_in_button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(log_in_button, 40);
    gtk_widget_set_size_request(log_in_button, 423, 53);

    widget_styling(log_in_button, current_screen, "auth_button");
    widget_styling(registration_success_label, current_screen, "registration_success_label");

    g_signal_connect(log_in_button, "clicked", G_CALLBACK(log_in_btn_clicked), NULL);
}
