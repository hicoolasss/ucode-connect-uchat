#include "../inc/client.h"

t_screen curent_screen;

static void signinbtn_clicked(GtkWidget *box) {
    gtk_widget_unparent(box);
    show_loginscreen();
}

void show_registrationscreen() {
    GtkWidget *box, *username, *password, *confirm_password;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(curent_screen.screen), box);

    GtkStyleContext *context = gtk_widget_get_style_context(curent_screen.screen);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    GtkWidget *welcome = gtk_label_new_with_mnemonic ("Welcome to the dark");

    GtkWidget *reg_button = gtk_button_new_with_mnemonic("Sign up");

    GtkWidget *have_account = gtk_label_new_with_mnemonic("Have account?");

    GtkWidget *signin = gtk_button_new_with_mnemonic("Sign in");

    //GtkWidget *signup_label = gtk_label_new_with_mnemonic("Sign up");


//    gtk_button_set_label(GTK_BUTTON(signup), "Sign up");

//    gtk_button_set_has_frame(GTK_BUTTON(signup), FALSE);


    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(box, 307);
    gtk_widget_set_margin_end(box, 307);
    gtk_widget_set_margin_top(box, 108);
    gtk_widget_set_margin_bottom(box, 108);

    gtk_widget_set_size_request(box, 586, 544);

//    gtk_window_set_child(GTK_WINDOW(curent_screen.screen), box);

    username = gtk_entry_new();
    password = gtk_entry_new();
    confirm_password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(password), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(confirm_password), 0.1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(username), "Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password), "Confirm Password");


    gtk_box_append(GTK_BOX(box), welcome);
    gtk_box_append(GTK_BOX(box), username);
    gtk_box_append(GTK_BOX(box), password);
    gtk_box_append(GTK_BOX(box), confirm_password);
    gtk_box_append(GTK_BOX(box), reg_button);
    gtk_box_append(GTK_BOX(box), have_account);
    gtk_box_append(GTK_BOX(box), signin);

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
    gtk_widget_set_margin_top(confirm_password, 30);

    gtk_widget_set_size_request(confirm_password, 423, 63);

    gtk_entry_set_visibility(GTK_ENTRY(confirm_password),FALSE);

    gtk_widget_set_margin_start(reg_button, 81);
    gtk_widget_set_margin_end(reg_button, 81);
    gtk_widget_set_margin_top(reg_button, 37);

    gtk_widget_set_size_request(reg_button, 423, 53);

    gtk_widget_set_margin_start(have_account, 163);
    gtk_widget_set_margin_end(have_account, 163);
    gtk_widget_set_margin_top(have_account, 5);

    gtk_widget_set_size_request(have_account, 260, 23);

    gtk_widget_set_margin_start(signin, 261);
    gtk_widget_set_margin_end(signin, 262);
    gtk_widget_set_margin_top(signin, 0);

    gtk_widget_set_size_request(signin, 63, 20);


    context = gtk_widget_get_style_context(box);

    gtk_style_context_add_class(context, "box");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(welcome);

    gtk_style_context_add_class(context, "welcome_to_the_dark");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    context = gtk_widget_get_style_context(username);

    gtk_style_context_add_class(context, "login");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(password);

    gtk_style_context_add_class(context, "login");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(confirm_password);


    gtk_style_context_add_class(context, "login");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(reg_button);

    gtk_style_context_add_class(context, "reg_button");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(have_account);

    gtk_style_context_add_class(context, "have_account");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(signin);

    gtk_style_context_add_class(context, "signin");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(curent_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

//    gtk_widget_show(reg_screen.screen);

    g_signal_connect(signin, "clicked", G_CALLBACK(signinbtn_clicked), NULL);

    gtk_window_present(GTK_WINDOW(curent_screen.screen));


}
