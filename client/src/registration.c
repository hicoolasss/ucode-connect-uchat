#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;
extern t_client cur_client;
t_registration cur_registration;



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

static void sign_inbtn_clicked()
{
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.log_in_conrainer), TRUE);
}

static void sign_up_btn_clicked(GtkWidget *widget, gpointer entries_array)
{
    bool is_registration_success = true;

    GtkWidget **entry_data = entries_array;

    GtkEntryBuffer *username = gtk_entry_get_buffer(GTK_ENTRY(entry_data[0]));
    GtkEntryBuffer *password1 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[1]));
    GtkEntryBuffer *password2 = gtk_entry_get_buffer(GTK_ENTRY(entry_data[2]));

    cur_client.login = mx_strdup(gtk_entry_buffer_get_text(username));
    cur_client.password = mx_strdup(gtk_entry_buffer_get_text(password1));
    const char *confirm_password = gtk_entry_buffer_get_text(password2);


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
    json_str = registration();
    send_message_to_server(json_str);

    if (is_registration_success) {
        
        set_unvisible_auth();
        gtk_widget_set_visible(GTK_WIDGET(curent_grid.log_in_conrainer), TRUE);
    }

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

    gtk_widget_set_halign(cur_registration.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(cur_registration.box, GTK_ALIGN_CENTER);

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
    
    // gtk_entry_set_visibility(GTK_ENTRY(cur_registration.password), FALSE);
    // gtk_entry_set_visibility(GTK_ENTRY(cur_registration.confirm_password),FALSE);

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

    gtk_widget_set_margin_start(cur_registration.welcome, 103);
    gtk_widget_set_margin_end(cur_registration.welcome, 99);
    gtk_widget_set_margin_top(cur_registration.welcome, 60);

    gtk_widget_set_size_request(cur_registration.welcome, 384, 35);

    gtk_widget_set_margin_start(cur_registration.username, 81);
    gtk_widget_set_margin_end(cur_registration.username, 81);
    gtk_widget_set_margin_top(cur_registration.username, 48);

    gtk_widget_set_size_request(cur_registration.username, 423, 63);

    gtk_widget_set_margin_start(cur_registration.username_error_label, 103);
    //gtk_widget_set_margin_end(cur_registration.username_error_label, 303);

    gtk_widget_set_margin_start(cur_registration.password, 81);
    gtk_widget_set_margin_end(cur_registration.password, 81);
    gtk_widget_set_margin_top(cur_registration.password, 5);

    gtk_widget_set_size_request(cur_registration.password, 423, 63);


    gtk_widget_set_margin_start(cur_registration.password_error_label, 103);
    gtk_widget_set_margin_end(cur_registration.password_error_label, 303);


    gtk_widget_set_margin_start(cur_registration.confirm_password, 81);
    gtk_widget_set_margin_end(cur_registration.confirm_password, 81);
    gtk_widget_set_margin_top(cur_registration.confirm_password, 5);

    gtk_widget_set_size_request(cur_registration.confirm_password, 423, 63);


    gtk_widget_set_margin_start(cur_registration.confirm_password_error_label, 103);
    gtk_widget_set_margin_end(cur_registration.confirm_password_error_label, 303);


    gtk_widget_set_margin_start(cur_registration.sign_up_btn, 81);
    gtk_widget_set_margin_end(cur_registration.sign_up_btn, 81);
    gtk_widget_set_margin_top(cur_registration.sign_up_btn, 5);

    gtk_widget_set_size_request(cur_registration.sign_up_btn, 423, 53);

    gtk_widget_set_margin_start(cur_registration.have_account, 163);
    gtk_widget_set_margin_end(cur_registration.have_account, 163);
    gtk_widget_set_margin_top(cur_registration.have_account, 12);

    gtk_widget_set_size_request(cur_registration.have_account, 260, 23);

    gtk_widget_set_margin_start(cur_registration.sign_in_btn, 262);
    gtk_widget_set_margin_end(cur_registration.sign_in_btn, 261);
    gtk_widget_set_margin_top(cur_registration.sign_in_btn, 0);

    gtk_widget_set_size_request(cur_registration.sign_in_btn, 63, 20);

    // get_username(GTK_ENTRY(username));
    // get_password(GTK_ENTRY(password));

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
