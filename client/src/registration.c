#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;
extern t_client cur_client;
t_registration cur_registration;


static void log_in_btn_clicked(void) {

    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.log_in_conrainer), TRUE);
    show_log_in();

}

static int get_username_status(void) {

    if (mx_strlen(cur_client.login) < 4) return -1;
    if (mx_strlen(cur_client.login) > 10) return -2;

    const char *forbidden_symbols = "$&=+<>,_`- ";
    
    for (int i = 0; i < mx_strlen(cur_client.login); i++) {
        
        for (int j = 0; j < 9; j++) {
            
            if (cur_client.login[i] == forbidden_symbols[j]) {
                
                return -3;
            
            }
        
        }
    
    }
    return 0;
}

static int get_password_status (const char *confirm_password) {

    if (mx_strlen(cur_client.password) < 4) return -1;
    if (mx_strlen(cur_client.password) > 10) return -2;

    const char *forbidden_symbols = "$&=+<>,_`- ";
    
    for (int i = 0; i < mx_strlen(cur_client.password); i++) {
        
        for (int j = 0; j < 9; j++) {
            
            if (cur_client.password[i] == forbidden_symbols[j]) {
                
                return -3;
            
            }
        
        }
    
    }

    if (mx_strcmp(cur_client.password, confirm_password) != 0) return -4;

    return 0;

}

static void sign_inbtn_clicked() {
    
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.log_in_conrainer), TRUE);

}

static void sign_up_btn_clicked(GtkWidget *widget, gpointer entries_array) {
    bool is_registration_success = true;

    GtkWidget **entry_data = entries_array;

    GtkEntryBuffer *username = gtk_entry_get_buffer(GTK_ENTRY(entry_data[0]));
    GtkEntryBuffer *password1 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[1]));
    GtkEntryBuffer *password2 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[2]));

    cur_client.login = mx_strdup(gtk_entry_buffer_get_text(username));
    cur_client.password = mx_strdup(gtk_entry_buffer_get_text(password1));
    const char *confirm_password = mx_strdup(gtk_entry_buffer_get_text(password2));

    int username_status = get_username_status();

    switch (username_status) {
        
        case 0: { 
            
            widget_restyling(cur_registration.username_error_label, curent_screen, "error_label", "hide_label");

            widget_restyling(entry_data[0], curent_screen, "wrong_auth_entry_field", "auth_entry_field");
            
            break;

        }

        case -1: {
            
            gtk_label_set_text(GTK_LABEL(cur_registration.username_error_label), "Username is too short");

            widget_restyling(cur_registration.username_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[0], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;
        
        }

        case -2: {

            gtk_label_set_text(GTK_LABEL(cur_registration.username_error_label), "Username is too long");

            widget_restyling(cur_registration.username_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[0], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;

        }

        case -3: {

            gtk_label_set_text(GTK_LABEL(cur_registration.username_error_label), "Username contain forbidden symbols!");

            widget_restyling(cur_registration.username_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[0], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;

        }


    }

    int password_status = get_password_status(confirm_password);

    switch (password_status) {
        
        case 0: {

            widget_restyling(cur_registration.password_error_label, curent_screen, "error_label", "hide_label");
            widget_restyling(cur_registration.confirm_password_error_label, curent_screen, "error_label", "hide_label");

            widget_restyling(entry_data[1], curent_screen, "wrong_auth_entry_field", "auth_entry_field");
            widget_restyling(entry_data[2], curent_screen, "wrong_auth_entry_field", "auth_entry_field");

            break;
        
        }

        case -1: {
            
            gtk_label_set_text(GTK_LABEL(cur_registration.password_error_label), "Password is too short");

            widget_restyling(cur_registration.password_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[1], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;
        
        }

        case -2: {

            gtk_label_set_text(GTK_LABEL(cur_registration.password_error_label), "Password is too long");

            widget_restyling(cur_registration.password_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[1], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;

        }

        case -3: {

            gtk_label_set_text(GTK_LABEL(cur_registration.password_error_label), "Password contain forbidden symbols!");

            widget_restyling(cur_registration.password_error_label, curent_screen, "hide_label", "error_label");

            widget_styling(entry_data[1], curent_screen, "wrong_auth_entry_field");

            is_registration_success = false;

            return;

        }


        case -4: {

            gtk_label_set_text(GTK_LABEL(cur_registration.password_error_label), "Passwords missmatch!");
            gtk_label_set_text(GTK_LABEL(cur_registration.confirm_password_error_label), "Passwords missmatch!");

            widget_styling(entry_data[1], curent_screen, "wrong_auth_entry_field");
            widget_styling(entry_data[2], curent_screen, "wrong_auth_entry_field");

            widget_restyling(cur_registration.password_error_label, curent_screen, "hide_label", "error_label");
            widget_restyling(cur_registration.confirm_password_error_label, curent_screen, "hide_label", "error_label");

            is_registration_success = false;

            return;

        }
    }

    char *json_str;
    json_str = registration(1);
    send_message_to_server(json_str);
    char buf[256];
    while (1)
    {
        int len = SSL_read(cur_client.ssl, buf, sizeof(buf));
        if (len < 0)
        {
            printf("Error: Unable to receive data from server\n");
        }
        else if (mx_strcmp(buf, "registered\n") == 0)
        {
            set_unvisible_auth();
            gtk_widget_set_visible(GTK_WIDGET(curent_grid.registration_success_container), TRUE);
            break;
        }
    }

    // if (is_registration_success) {
        
    //     set_unvisible_auth();
    //     gtk_widget_set_visible(GTK_WIDGET(curent_grid.registration_success_container), TRUE);
    
    // }

}

void show_registration()
{

    //GtkWidget *box, *username, *wrong_username, *password, *wrong_password, *confirm_password, *wrong_password1;
    cur_registration.username_error_label = gtk_label_new("label");

    cur_registration.password_error_label = gtk_label_new("label");

    cur_registration.confirm_password_error_label = gtk_label_new("label");

    cur_registration.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    cur_registration.welcome = gtk_label_new_with_mnemonic("Welcome to the dark!");

    cur_registration.sign_up_btn = gtk_button_new_with_mnemonic("Sign up");

    cur_registration.have_account = gtk_label_new_with_mnemonic("Have account?");

    cur_registration.sign_in_btn = gtk_button_new_with_label("Sign in");

    gtk_grid_attach(GTK_GRID(curent_grid.registration_container), cur_registration.box, 0, 0, 1, 1);

    gtk_widget_set_margin_start(cur_registration.box, 307);
    gtk_widget_set_margin_end(cur_registration.box, 307);
    gtk_widget_set_margin_top(cur_registration.box, 108);
    gtk_widget_set_margin_bottom(cur_registration.box, 108);

    gtk_widget_set_size_request(cur_registration.box, 586, 544);

    cur_registration.username = gtk_entry_new();
    cur_registration.password = gtk_entry_new();
    cur_registration.confirm_password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(cur_registration.username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(cur_registration.password), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(cur_registration.confirm_password), 0.1);
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(cur_registration.username), " Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(cur_registration.password), " Password");
    gtk_entry_set_placeholder_text(GTK_ENTRY(cur_registration.confirm_password), "  Confirm Password");
    

    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.welcome);

    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.username);
    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.username_error_label);

    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.password);
    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.password_error_label);

    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.confirm_password);
    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.confirm_password_error_label);

    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.sign_up_btn);
    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.have_account);
    gtk_box_append(GTK_BOX(cur_registration.box), cur_registration.sign_in_btn);

    gtk_widget_set_halign(cur_registration.welcome, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.welcome, 60);

    gtk_widget_set_halign(cur_registration.username, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.username, 48);
    gtk_widget_set_size_request(cur_registration.username, 423, 63);

    gtk_widget_set_halign(cur_registration.username_error_label, GTK_ALIGN_CENTER);


    gtk_widget_set_halign(cur_registration.password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.password, 5);
    gtk_widget_set_size_request(cur_registration.password, 423, 63);

    gtk_widget_set_halign(cur_registration.password_error_label, GTK_ALIGN_CENTER);


    gtk_widget_set_halign(cur_registration.confirm_password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.confirm_password, 5);
    gtk_widget_set_size_request(cur_registration.confirm_password, 423, 63);

    gtk_widget_set_halign(cur_registration.confirm_password_error_label, GTK_ALIGN_CENTER);


    gtk_widget_set_halign(cur_registration.sign_up_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.sign_up_btn, 5);
    gtk_widget_set_size_request(cur_registration.sign_up_btn, 423, 53);

    gtk_widget_set_halign(cur_registration.have_account, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.have_account, 12);
    gtk_widget_set_size_request(cur_registration.have_account, 260, 23);

    gtk_widget_set_halign(cur_registration.sign_in_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(cur_registration.sign_in_btn, 0);
    gtk_widget_set_size_request(cur_registration.sign_in_btn, 63, 20);


    widget_styling(cur_registration.box, curent_screen, "auth_main_box");
    widget_styling(cur_registration.welcome, curent_screen, "auth_welcome_to_the_dark");
    widget_styling(cur_registration.username, curent_screen, "auth_entry_field");
    widget_styling(cur_registration.password, curent_screen, "auth_entry_field");
    widget_styling(cur_registration.confirm_password, curent_screen, "auth_entry_field");
    widget_styling(cur_registration.sign_up_btn, curent_screen, "auth_button");
    widget_styling(cur_registration.sign_in_btn, curent_screen, "auth_sign_button");
    widget_styling(cur_registration.have_account, curent_screen, "auth_dont_or_have_account");
    
    widget_styling(cur_registration.username_error_label, curent_screen, "hide_label");
    widget_styling(cur_registration.password_error_label, curent_screen, "hide_label");
    widget_styling(cur_registration.confirm_password_error_label, curent_screen, "hide_label");

    g_signal_connect(cur_registration.sign_in_btn, "clicked", G_CALLBACK(sign_inbtn_clicked), NULL);

    GtkWidget **entry_arr = (GtkWidget **)malloc(3 * sizeof(GtkWidget *)); //{LOGIN_entry_field1, LOGIN_entry_field2};
    entry_arr[0] = cur_registration.username;
    entry_arr[1] = cur_registration.password;
    entry_arr[2] = cur_registration.confirm_password;

    g_signal_connect(cur_registration.sign_up_btn, "clicked", G_CALLBACK(sign_up_btn_clicked), entry_arr);

    gtk_window_present(GTK_WINDOW(curent_screen.screen));

}


void show_success_registration(void) {

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *registration_success_label = gtk_label_new("Registration completed successfully!");

    GtkWidget *log_in_button = gtk_button_new_with_mnemonic("Log in");

    gtk_grid_attach(GTK_GRID(curent_grid.registration_success_container), box, 0, 0, 1, 1);

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

    widget_styling(log_in_button, curent_screen, "auth_button");
    widget_styling(registration_success_label, curent_screen, "registration_success_label");

    g_signal_connect(log_in_button, "clicked", G_CALLBACK(log_in_btn_clicked), NULL);


}
