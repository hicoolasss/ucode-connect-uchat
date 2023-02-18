#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

static void sign_inbtn_clicked() {
    set_unvisible_auth();
    gtk_widget_set_visible(GTK_WIDGET(curent_grid.log_in_conrainer), TRUE);
}


static void sign_up_btn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_home();
}

void show_registration() {
    
    GtkWidget *box, *username, *password, *confirm_password;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *welcome = gtk_label_new_with_mnemonic ("Welcome to the dark!");

    GtkWidget *sign_up_button_registration = gtk_button_new_with_mnemonic("Sign up");

    GtkWidget *have_account = gtk_label_new_with_mnemonic("Have account?");

    GtkWidget *sign_in_button = gtk_button_new_with_label("Sign in");

    gtk_grid_attach(GTK_GRID(curent_grid.registration_container), box, 0, 0, 1, 1);


    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(box, 307);
    gtk_widget_set_margin_end(box, 307);
    gtk_widget_set_margin_top(box, 108);
    gtk_widget_set_margin_bottom(box, 108);

    gtk_widget_set_size_request(box, 586, 544);

    username = gtk_entry_new();
    password = gtk_entry_new();
    confirm_password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(password), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(confirm_password), 0.1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(username), " Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), " Password");
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password), "  Confirm Password");

    gtk_box_append(GTK_BOX(box), welcome);
    gtk_box_append(GTK_BOX(box), username);
    gtk_box_append(GTK_BOX(box), password);
    gtk_box_append(GTK_BOX(box), confirm_password);
    gtk_box_append(GTK_BOX(box), sign_up_button_registration);
    gtk_box_append(GTK_BOX(box), have_account);
    gtk_box_append(GTK_BOX(box), sign_in_button);

    gtk_widget_set_margin_start(welcome, 103);
    gtk_widget_set_margin_end(welcome, 99);
    gtk_widget_set_margin_top(welcome, 60);

    gtk_widget_set_size_request(welcome, 384, 35);

    gtk_widget_set_margin_start(username, 81);
    gtk_widget_set_margin_end(username, 81);
    gtk_widget_set_margin_top(username, 48);

    gtk_widget_set_size_request(username, 423, 63);

    gtk_widget_set_margin_start(password, 81);
    gtk_widget_set_margin_end(password, 81);
    gtk_widget_set_margin_top(password, 25);

    gtk_widget_set_size_request(password, 423, 63);

    gtk_entry_set_visibility(GTK_ENTRY(password),FALSE);

    gtk_widget_set_margin_start(confirm_password, 81);
    gtk_widget_set_margin_end(confirm_password, 81);
    gtk_widget_set_margin_top(confirm_password, 25);

    gtk_widget_set_size_request(confirm_password, 423, 63);

    gtk_entry_set_visibility(GTK_ENTRY(confirm_password),FALSE);

    gtk_widget_set_margin_start(sign_up_button_registration, 81);
    gtk_widget_set_margin_end(sign_up_button_registration, 81);
    gtk_widget_set_margin_top(sign_up_button_registration, 25);

    gtk_widget_set_size_request(sign_up_button_registration, 423, 53);

    gtk_widget_set_margin_start(have_account, 163);
    gtk_widget_set_margin_end(have_account, 163);
    gtk_widget_set_margin_top(have_account, 12);

    gtk_widget_set_size_request(have_account, 260, 23);

    gtk_widget_set_margin_start(sign_in_button, 262);
    gtk_widget_set_margin_end(sign_in_button, 261);
    gtk_widget_set_margin_top(sign_in_button, 0);

    gtk_widget_set_size_request(sign_in_button, 63, 20);


    widget_styling(box, curent_screen, "auth_main_box");
    widget_styling(welcome, curent_screen, "auth_welcome_to_the_dark");
    widget_styling(username, curent_screen, "auth_entry_field");
    widget_styling(password, curent_screen, "auth_entry_field");
    widget_styling(confirm_password, curent_screen, "auth_entry_field");
    widget_styling(sign_up_button_registration, curent_screen, "auth_button");
    widget_styling(sign_in_button, curent_screen, "auth_sign_button");
    widget_styling(have_account, curent_screen, "auth_dont_or_have_account");

    g_signal_connect(sign_in_button, "clicked", G_CALLBACK(sign_inbtn_clicked), NULL);
    g_signal_connect(sign_up_button_registration, "clicked", G_CALLBACK(sign_up_btn_clicked), NULL);

    gtk_window_present(GTK_WINDOW(curent_screen.screen));


}
