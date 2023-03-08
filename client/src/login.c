#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

static void donthaveaccountbtn_clicked() {
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.registration_container), TRUE);

}

static void log_in_btn_clicked(GtkWidget *widget, gpointer data) {

    curent_grid.is_log_in_clicked = TRUE;

    GtkWidget **entry_data = data;
    GtkEntryBuffer *username = gtk_entry_get_buffer(GTK_ENTRY(entry_data[0]));
    GtkEntryBuffer *password = gtk_entry_get_buffer(GTK_ENTRY(entry_data[1]));

    cur_client.login = mx_strdup(gtk_entry_buffer_get_text(username));
    cur_client.password = mx_strdup(gtk_entry_buffer_get_text(password));
    
    if (mx_strlen(cur_client.login) == 0) {
        //erro
        return;
    }

    if (mx_strlen(cur_client.password) == 0) {
        //erro
        return;
    }
    
    // char *json_str;
    // json_str = registration();
    // send_message_to_server(json_str);
   
    // char *buf = NULL;
    
    // int len = SSL_read(cur_client.ssl, buf, mx_strlen(buf));
       
    // if (len < 0) {

    //     printf("Error: Unable to receive data from server\n");
        
    // } else if (mx_strcmp(buf, "incorrect password\n") == 0) {
            
    //     return;

    // } else if (mx_strcmp(buf, "success\n") == 0) {
            
        set_unvisible_auth();
        show_home();
        
    //}

}

void show_login() {

    GtkWidget *box, *username, *password;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *welcome = gtk_label_new_with_mnemonic ("Welcome to the dark!");

    GtkWidget *log_in_button = gtk_button_new_with_mnemonic("Log in");

    GtkWidget *dont_have_account = gtk_label_new_with_mnemonic("Don’t have an account?");

    GtkWidget *sign_up_button_log_in = gtk_button_new_with_label("Sign up");

    gtk_grid_attach(GTK_GRID(curent_grid.log_in_conrainer), box, 0, 0, 1, 1);

    gtk_widget_set_margin_start(box, 307);
    gtk_widget_set_margin_end(box, 307);
    gtk_widget_set_margin_top(box, 108);
    gtk_widget_set_margin_bottom(box, 108);

    gtk_widget_set_size_request(box, 586, 544);

    username = gtk_entry_new();
    password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(password), 0.1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(username), " Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), " Password");
    gtk_entry_set_visibility(GTK_ENTRY(password),FALSE);


    gtk_box_append(GTK_BOX(box), welcome);
    gtk_box_append(GTK_BOX(box), username);
    gtk_box_append(GTK_BOX(box), password);
    gtk_box_append(GTK_BOX(box), log_in_button);
    gtk_box_append(GTK_BOX(box), dont_have_account);
    gtk_box_append(GTK_BOX(box), sign_up_button_log_in);

    gtk_widget_set_halign(welcome, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(welcome, 60);


    gtk_widget_set_halign(username, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(username, 48);
    gtk_widget_set_size_request(username, 423, 63);


    gtk_widget_set_halign(password, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(password, 25);
    gtk_widget_set_size_request(password, 423, 63);


    gtk_widget_set_halign(log_in_button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(log_in_button, 37);
    gtk_widget_set_size_request(log_in_button, 423, 53);


    gtk_widget_set_halign(dont_have_account, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(dont_have_account, 25);
    gtk_widget_set_size_request(dont_have_account, 260, 23);


    gtk_widget_set_halign(sign_up_button_log_in, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(sign_up_button_log_in, 0);
    gtk_widget_set_size_request(sign_up_button_log_in, 63, 20);


    widget_styling(box, curent_screen, "auth_main_box");
    widget_styling(welcome, curent_screen, "auth_welcome_to_the_dark");
    widget_styling(username, curent_screen, "auth_entry_field");
    widget_styling(password, curent_screen, "auth_entry_field");
    widget_styling(log_in_button, curent_screen, "auth_button");
    widget_styling(dont_have_account, curent_screen, "auth_dont_or_have_account");
    widget_styling(sign_up_button_log_in, curent_screen, "auth_sign_button");

    GtkWidget **entry_arr = (GtkWidget **)malloc(2 * sizeof(GtkWidget *)); //{LOGIN_entry_field1, LOGIN_entry_field2};
    entry_arr[0] = username;
    entry_arr[1] = password;

    g_signal_connect(sign_up_button_log_in, "clicked", G_CALLBACK(donthaveaccountbtn_clicked), NULL);

    g_signal_connect(log_in_button, "clicked", G_CALLBACK(log_in_btn_clicked), entry_arr);
}
