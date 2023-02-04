#include "../inc/client.h"

extern t_screen login_screen;

void show_loginscreen () {
    
    GtkWidget *box, *username, *password;

    GtkStyleContext *context = gtk_widget_get_style_context(login_screen.screen);
    
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *welcome = gtk_label_new_with_mnemonic ("Welcome to the dark");

    GtkWidget *login_button = gtk_button_new_with_mnemonic("Login");

    GtkWidget *dont_have_account = gtk_label_new_with_mnemonic("Don’t have an account?");

    GtkWidget *signup = gtk_label_new_with_mnemonic("Sign up");

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

    gtk_window_set_child(GTK_WINDOW(login_screen.screen), box);

    username = gtk_entry_new();
    password = gtk_entry_new();

    gtk_entry_set_alignment(GTK_ENTRY(username), 0.1);
    gtk_entry_set_alignment(GTK_ENTRY(password), 0.1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(username), "Username");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");


    gtk_box_append(GTK_BOX(box), welcome);
    gtk_box_append(GTK_BOX(box), username);
    gtk_box_append(GTK_BOX(box), password);
    gtk_box_append(GTK_BOX(box), login_button);
    gtk_box_append(GTK_BOX(box), dont_have_account);
    gtk_box_append(GTK_BOX(box), signup);

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

    gtk_widget_set_margin_start(login_button, 81);
    gtk_widget_set_margin_end(login_button, 81);
    gtk_widget_set_margin_top(login_button, 37);

    gtk_widget_set_size_request(login_button, 423, 53);

    gtk_widget_set_margin_start(dont_have_account, 163);
    gtk_widget_set_margin_end(dont_have_account, 163);
    gtk_widget_set_margin_top(dont_have_account, 25);

    gtk_widget_set_size_request(dont_have_account, 260, 23);

    gtk_widget_set_margin_start(signup, 261);
    gtk_widget_set_margin_end(signup, 262);
    gtk_widget_set_margin_top(signup, 0);

    gtk_widget_set_size_request(signup, 63, 20);


    widget_styling(box, login_screen, "box");

    context = gtk_widget_get_style_context(welcome);

    gtk_style_context_add_class(context, "welcome_to_the_dark");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    context = gtk_widget_get_style_context(username);

    gtk_style_context_add_class(context, "login");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(password);

    gtk_style_context_add_class(context, "login");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(login_button);

    gtk_style_context_add_class(context, "login_button");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(dont_have_account);

    gtk_style_context_add_class(context, "dont_have_account");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(signup);

    gtk_style_context_add_class(context, "signup");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(login_screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show(login_screen.screen);
}
