#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;

t_achievements current_achievements;

static void scrolled_window_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class)
{

    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, name_of_css_class);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(screen.provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void show_achievements()
{

    GtkWidget *achievements_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *first_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *second_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *achievements_icon_btn = gtk_button_new();

    GtkWidget *achievements_label_btn = gtk_button_new_with_label("Achievements"); ////

    // completed and not completed yet buttons
    GtkWidget *btn_0 = gtk_button_new();

    GtkWidget *btn_1 = gtk_button_new();

    GtkWidget *btn_2 = gtk_button_new();

    GtkWidget *btn_3 = gtk_button_new();

    GtkWidget *btn_4 = gtk_button_new();

    GtkWidget *btn_5 = gtk_button_new();

    GtkWidget *btn_6 = gtk_button_new();

    GtkWidget *btn_7 = gtk_button_new();

    // first box
    GtkWidget *first_step_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *first_step_label = gtk_label_new("First Step!");

    GtkWidget *first_step_desc_label = gtk_label_new("Write your first message");

    // second box
    GtkWidget *explorer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *explorer_label = gtk_label_new("Explorer");

    GtkWidget *explorer_desc_label = gtk_label_new("Open this folder");

    // third box
    GtkWidget *fickle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *fickle_label = gtk_label_new("Fickle");

    GtkWidget *fickle_desc_label = gtk_label_new("Change your avatar");

    // fourth box
    GtkWidget *argument_with_a_women_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *argument_with_a_women_label = gtk_label_new("Argument with a women");

    GtkWidget *argument_with_a_women_desc_label = gtk_label_new("Write 5000 messages");

    // fifth box
    GtkWidget *loving_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *loving_label = gtk_label_new("Loving");

    GtkWidget *loving_desc_label = gtk_label_new("Confess your love");

    // 6th box
    GtkWidget *vicious_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *vicious_label = gtk_label_new("Vicious");

    GtkWidget *vicious_desc_label = gtk_label_new("Block someone");

    // 7th box
    GtkWidget *smart_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *smart_label = gtk_label_new("Smart");

    GtkWidget *smart_desc_label = gtk_label_new("Press achievements button");

    // 8th box
    GtkWidget *mission_impossible_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *mission_impossible_label = gtk_label_new("Mission impossible");

    GtkWidget *mission_impossible_desc_label = gtk_label_new("Complete all achievements");

    // scrolled_window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), achievements_box);
    gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), false);
    gtk_scrolled_window_set_propagate_natural_width(GTK_SCROLLED_WINDOW(scrolled_window), true);

    // attach your main box to mygrid
    gtk_grid_attach(GTK_GRID(current_grid.achievements), scrolled_window, 0, 0, 1, 1);

    gtk_widget_set_halign(scrolled_window, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_CENTER);

    gtk_widget_set_size_request(scrolled_window, 1010, 667);

    GtkWidget *scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(scrolled_window));

    // achievements_icon_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_icon_btn);

    gtk_widget_set_margin_start(achievements_icon_btn, 405);
    gtk_widget_set_margin_end(achievements_icon_btn, 405);
    gtk_widget_set_margin_top(achievements_icon_btn, 11);

    gtk_widget_set_size_request(achievements_icon_btn, 200, 200);

    // achievements_label_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_label_btn);

    gtk_widget_set_margin_start(achievements_label_btn, 425);
    gtk_widget_set_margin_end(achievements_label_btn, 425);
    gtk_widget_set_margin_top(achievements_label_btn, 32);

    gtk_widget_set_size_request(achievements_label_btn, 130, 40);

    // first_hbox
    gtk_box_append(GTK_BOX(achievements_box), first_hbox);

    gtk_widget_set_margin_start(first_hbox, 0);
    gtk_widget_set_margin_end(first_hbox, 0);
    gtk_widget_set_margin_top(first_hbox, 32);

    gtk_widget_set_size_request(first_hbox, 1010, 254);

    // first_step_box
    gtk_box_append(GTK_BOX(first_hbox), first_step_box);

    gtk_widget_set_margin_start(first_step_box, 17);
    gtk_widget_set_margin_top(first_step_box, 0);
    gtk_widget_set_margin_bottom(first_step_box, 0);

    gtk_widget_set_size_request(first_step_box, 190, 254);

    // label
    gtk_widget_set_halign(first_step_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), first_step_label);

    gtk_widget_set_margin_top(first_step_label, 46);

    // desc
    gtk_widget_set_halign(first_step_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), first_step_desc_label);

    gtk_widget_set_margin_top(first_step_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_0, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), btn_0);

    gtk_widget_set_margin_top(btn_0, 35);

    gtk_widget_set_size_request(btn_0, 50, 50);

    // explorer_box
    gtk_box_append(GTK_BOX(first_hbox), explorer_box);

    gtk_widget_set_halign(explorer_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(explorer_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(explorer_box, 72);
    gtk_widget_set_margin_top(explorer_box, 0);
    gtk_widget_set_margin_bottom(explorer_box, 0);

    gtk_widget_set_size_request(explorer_box, 190, 254);

    // label
    gtk_widget_set_halign(explorer_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), explorer_label);

    gtk_widget_set_margin_top(explorer_label, 46);

    // desc
    gtk_widget_set_halign(explorer_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), explorer_desc_label);

    gtk_widget_set_margin_top(explorer_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_1, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), btn_1);

    gtk_widget_set_margin_top(btn_1, 35);

    gtk_widget_set_size_request(btn_1, 50, 50);

    // fickle_box
    gtk_box_append(GTK_BOX(first_hbox), fickle_box);

    gtk_widget_set_halign(fickle_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(fickle_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(fickle_box, 72);
    gtk_widget_set_margin_top(fickle_box, 0);
    gtk_widget_set_margin_bottom(fickle_box, 0);

    gtk_widget_set_size_request(fickle_box, 190, 254);

    // label
    gtk_widget_set_halign(fickle_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), fickle_label);

    gtk_widget_set_margin_top(fickle_label, 46);

    // desc
    gtk_widget_set_halign(fickle_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), fickle_desc_label);

    gtk_widget_set_margin_top(fickle_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_2, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), btn_2);

    gtk_widget_set_margin_top(btn_2, 35);

    gtk_widget_set_size_request(btn_2, 50, 50);

    // argument_with_a_women_box
    gtk_box_append(GTK_BOX(first_hbox), argument_with_a_women_box);

    gtk_widget_set_halign(argument_with_a_women_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(argument_with_a_women_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(argument_with_a_women_box, 72);
    gtk_widget_set_margin_top(argument_with_a_women_box, 0);
    gtk_widget_set_margin_bottom(argument_with_a_women_box, 0);

    gtk_widget_set_size_request(argument_with_a_women_box, 190, 254);

    // label
    gtk_widget_set_halign(argument_with_a_women_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), argument_with_a_women_label);

    gtk_widget_set_margin_top(argument_with_a_women_label, 46);

    // desc
    gtk_widget_set_halign(argument_with_a_women_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), argument_with_a_women_desc_label);

    gtk_widget_set_margin_top(argument_with_a_women_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_3, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), btn_3);

    gtk_widget_set_margin_top(btn_3, 35);

    gtk_widget_set_size_request(btn_3, 50, 50);

    // second_hbox
    gtk_box_append(GTK_BOX(achievements_box), second_hbox);

    gtk_widget_set_halign(second_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(second_hbox, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(second_hbox, 0);
    gtk_widget_set_margin_end(second_hbox, 0);
    gtk_widget_set_margin_top(second_hbox, 32);
    gtk_widget_set_margin_bottom(second_hbox, 32);

    gtk_widget_set_size_request(second_hbox, 1010, 254);

    // loving_box
    gtk_box_append(GTK_BOX(second_hbox), loving_box);

    gtk_widget_set_halign(loving_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(loving_box, 17);
    gtk_widget_set_margin_top(loving_box, 0);
    gtk_widget_set_margin_bottom(loving_box, 0);

    gtk_widget_set_size_request(loving_box, 190, 254);

    // label
    gtk_widget_set_halign(loving_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), loving_label);

    gtk_widget_set_margin_top(loving_label, 46);

    // desc
    gtk_widget_set_halign(loving_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), loving_desc_label);

    gtk_widget_set_margin_top(loving_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_4, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), btn_4);

    gtk_widget_set_margin_top(btn_4, 35);

    gtk_widget_set_size_request(btn_4, 50, 50);

    // vicious_box
    gtk_box_append(GTK_BOX(second_hbox), vicious_box);

    gtk_widget_set_halign(vicious_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vicious_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(vicious_box, 72);
    gtk_widget_set_margin_top(vicious_box, 0);
    gtk_widget_set_margin_bottom(vicious_box, 0);

    gtk_widget_set_size_request(vicious_box, 190, 254);

    // label
    gtk_widget_set_halign(vicious_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), vicious_label);

    gtk_widget_set_margin_top(vicious_label, 46);

    // desc
    gtk_widget_set_halign(vicious_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), vicious_desc_label);

    gtk_widget_set_margin_top(vicious_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_5, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), btn_5);

    gtk_widget_set_margin_top(btn_5, 35);

    gtk_widget_set_size_request(btn_5, 50, 50);

    // smart_box
    gtk_box_append(GTK_BOX(second_hbox), smart_box);

    gtk_widget_set_halign(smart_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(smart_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(smart_box, 72);
    gtk_widget_set_margin_top(smart_box, 0);
    gtk_widget_set_margin_bottom(smart_box, 0);

    gtk_widget_set_size_request(smart_box, 190, 254);

    // label
    gtk_widget_set_halign(smart_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), smart_label);

    gtk_widget_set_margin_top(smart_label, 46);

    // desc
    gtk_widget_set_halign(smart_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), smart_desc_label);

    gtk_widget_set_margin_top(smart_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_6, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), btn_6);

    gtk_widget_set_margin_top(btn_6, 35);

    gtk_widget_set_size_request(btn_6, 50, 50);

    // mission_impossible_box
    gtk_box_append(GTK_BOX(second_hbox), mission_impossible_box);

    gtk_widget_set_halign(mission_impossible_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mission_impossible_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(mission_impossible_box, 72);
    gtk_widget_set_margin_top(mission_impossible_box, 0);
    gtk_widget_set_margin_bottom(mission_impossible_box, 0);

    gtk_widget_set_size_request(mission_impossible_box, 190, 254);

    // label
    gtk_widget_set_halign(mission_impossible_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), mission_impossible_label);

    gtk_widget_set_margin_top(mission_impossible_label, 46);

    // desc
    gtk_widget_set_halign(mission_impossible_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), mission_impossible_desc_label);

    gtk_widget_set_margin_top(mission_impossible_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_7, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), btn_7);

    gtk_widget_set_margin_top(btn_7, 35);

    gtk_widget_set_size_request(btn_7, 50, 50);

    // styling
    widget_styling(achievements_box, current_screen, "achievements_box");
    scrolled_window_styling(scrollbar, current_screen, "scrollbar");
    widget_styling(achievements_icon_btn, current_screen, "achievements_icon_btn");
    widget_styling(achievements_label_btn, current_screen, "achievements_label_btn");

    widget_styling(first_hbox, current_screen, "first_hbox");

    widget_styling(first_step_box, current_screen, "achievement_card");
    widget_styling(first_step_label, current_screen, "achievement_label_text");
    widget_styling(first_step_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.first_step)
    {
        widget_styling(btn_0, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_0, current_screen, "unfinished_btn");
    }

    widget_styling(explorer_box, current_screen, "achievement_card");
    widget_styling(explorer_label, current_screen, "achievement_label_text");
    widget_styling(explorer_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_1, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_1, current_screen, "unfinished_btn");
    //}

    widget_styling(fickle_box, current_screen, "achievement_card");
    widget_styling(fickle_label, current_screen, "achievement_label_text");
    widget_styling(fickle_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_2, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_2, current_screen, "unfinished_btn");
    //}

    widget_styling(argument_with_a_women_box, current_screen, "achievement_card");
    widget_styling(argument_with_a_women_label, current_screen, "achievement_label_text");
    widget_styling(argument_with_a_women_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_3, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_3, current_screen, "unfinished_btn");
    //}

    widget_styling(loving_box, current_screen, "achievement_card");
    widget_styling(loving_label, current_screen, "achievement_label_text");
    widget_styling(loving_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_4, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_4, current_screen, "unfinished_btn");
    //}

    widget_styling(vicious_box, current_screen, "achievement_card");
    widget_styling(vicious_label, current_screen, "achievement_label_text");
    widget_styling(vicious_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_5, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_5, current_screen, "unfinished_btn");
    //}

    widget_styling(smart_box, current_screen, "achievement_card");
    widget_styling(smart_label, current_screen, "achievement_label_text");
    widget_styling(smart_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_6, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_6, current_screen, "unfinished_btn");
    //}

    widget_styling(mission_impossible_box, current_screen, "achievement_card");
    widget_styling(mission_impossible_label, current_screen, "achievement_label_text");
    widget_styling(mission_impossible_desc_label, current_screen, "achievement_desc_text");

    // if () {
    // widget_styling(btn_7, current_screen, "complete_btn");
    // } else {
    widget_styling(btn_7, current_screen, "unfinished_btn");
    //}

    widget_styling(second_hbox, current_screen, "second_hbox");
    widget_styling(loving_box, current_screen, "achievement_card");
}

void update_show_achievements()
{
    GtkWidget *children, *iter;

    children = gtk_widget_get_first_child(current_grid.achievements);

    for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.achievements))
    {

        gtk_widget_unparent(iter);
    }

    mx_printstr("update achievements\n");

    GtkWidget *achievements_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *first_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *second_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *achievements_icon_btn = gtk_button_new();

    GtkWidget *achievements_label_btn = gtk_button_new_with_label("Achievements"); ////

    // completed and not completed yet buttons
    GtkWidget *btn_0 = gtk_button_new();

    GtkWidget *btn_1 = gtk_button_new();

    GtkWidget *btn_2 = gtk_button_new();

    GtkWidget *btn_3 = gtk_button_new();

    GtkWidget *btn_4 = gtk_button_new();

    GtkWidget *btn_5 = gtk_button_new();

    GtkWidget *btn_6 = gtk_button_new();

    GtkWidget *btn_7 = gtk_button_new();

    // first box
    GtkWidget *first_step_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *first_step_label = gtk_label_new("First Step!");

    GtkWidget *first_step_desc_label = gtk_label_new("Write your first message");

    // second box
    GtkWidget *explorer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *explorer_label = gtk_label_new("Explorer");

    GtkWidget *explorer_desc_label = gtk_label_new("Open this folder");

    // third box
    GtkWidget *fickle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *fickle_label = gtk_label_new("Fickle");

    GtkWidget *fickle_desc_label = gtk_label_new("Change your avatar");

    // fourth box
    GtkWidget *argument_with_a_women_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *argument_with_a_women_label = gtk_label_new("Argument with a women");

    GtkWidget *argument_with_a_women_desc_label = gtk_label_new("Write 5000 messages");

    // fifth box
    GtkWidget *loving_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *loving_label = gtk_label_new("Loving");

    GtkWidget *loving_desc_label = gtk_label_new("Confess your love");

    // 6th box
    GtkWidget *vicious_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *vicious_label = gtk_label_new("Vicious");

    GtkWidget *vicious_desc_label = gtk_label_new("Block someone");

    // 7th box
    GtkWidget *smart_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *smart_label = gtk_label_new("Smart");

    GtkWidget *smart_desc_label = gtk_label_new("Press achievements button");

    // 8th box
    GtkWidget *mission_impossible_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *mission_impossible_label = gtk_label_new("Mission impossible");

    GtkWidget *mission_impossible_desc_label = gtk_label_new("Complete all achievements");

    // scrolled_window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), achievements_box);
    gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), false);
    gtk_scrolled_window_set_propagate_natural_width(GTK_SCROLLED_WINDOW(scrolled_window), true);

    // attach your main box to mygrid
    gtk_grid_attach(GTK_GRID(current_grid.achievements), scrolled_window, 0, 0, 1, 1);

    gtk_widget_set_halign(scrolled_window, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_CENTER);

    gtk_widget_set_size_request(scrolled_window, 1010, 667);

    GtkWidget *scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(scrolled_window));

    // achievements_icon_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_icon_btn);

    gtk_widget_set_margin_start(achievements_icon_btn, 405);
    gtk_widget_set_margin_end(achievements_icon_btn, 405);
    gtk_widget_set_margin_top(achievements_icon_btn, 11);

    gtk_widget_set_size_request(achievements_icon_btn, 200, 200);

    // achievements_label_btn
    gtk_box_append(GTK_BOX(achievements_box), achievements_label_btn);

    gtk_widget_set_margin_start(achievements_label_btn, 425);
    gtk_widget_set_margin_end(achievements_label_btn, 425);
    gtk_widget_set_margin_top(achievements_label_btn, 32);

    gtk_widget_set_size_request(achievements_label_btn, 130, 40);

    // first_hbox
    gtk_box_append(GTK_BOX(achievements_box), first_hbox);

    gtk_widget_set_margin_start(first_hbox, 0);
    gtk_widget_set_margin_end(first_hbox, 0);
    gtk_widget_set_margin_top(first_hbox, 32);

    gtk_widget_set_size_request(first_hbox, 1010, 254);

    // first_step_box
    gtk_box_append(GTK_BOX(first_hbox), first_step_box);

    gtk_widget_set_margin_start(first_step_box, 17);
    gtk_widget_set_margin_top(first_step_box, 0);
    gtk_widget_set_margin_bottom(first_step_box, 0);

    gtk_widget_set_size_request(first_step_box, 190, 254);

    // label
    gtk_widget_set_halign(first_step_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), first_step_label);

    gtk_widget_set_margin_top(first_step_label, 46);

    // desc
    gtk_widget_set_halign(first_step_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), first_step_desc_label);

    gtk_widget_set_margin_top(first_step_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_0, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(first_step_box), btn_0);

    gtk_widget_set_margin_top(btn_0, 35);

    gtk_widget_set_size_request(btn_0, 50, 50);

    // explorer_box
    gtk_box_append(GTK_BOX(first_hbox), explorer_box);

    gtk_widget_set_halign(explorer_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(explorer_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(explorer_box, 72);
    gtk_widget_set_margin_top(explorer_box, 0);
    gtk_widget_set_margin_bottom(explorer_box, 0);

    gtk_widget_set_size_request(explorer_box, 190, 254);

    // label
    gtk_widget_set_halign(explorer_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), explorer_label);

    gtk_widget_set_margin_top(explorer_label, 46);

    // desc
    gtk_widget_set_halign(explorer_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), explorer_desc_label);

    gtk_widget_set_margin_top(explorer_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_1, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(explorer_box), btn_1);

    gtk_widget_set_margin_top(btn_1, 35);

    gtk_widget_set_size_request(btn_1, 50, 50);

    // fickle_box
    gtk_box_append(GTK_BOX(first_hbox), fickle_box);

    gtk_widget_set_halign(fickle_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(fickle_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(fickle_box, 72);
    gtk_widget_set_margin_top(fickle_box, 0);
    gtk_widget_set_margin_bottom(fickle_box, 0);

    gtk_widget_set_size_request(fickle_box, 190, 254);

    // label
    gtk_widget_set_halign(fickle_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), fickle_label);

    gtk_widget_set_margin_top(fickle_label, 46);

    // desc
    gtk_widget_set_halign(fickle_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), fickle_desc_label);

    gtk_widget_set_margin_top(fickle_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_2, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(fickle_box), btn_2);

    gtk_widget_set_margin_top(btn_2, 35);

    gtk_widget_set_size_request(btn_2, 50, 50);

    // argument_with_a_women_box
    gtk_box_append(GTK_BOX(first_hbox), argument_with_a_women_box);

    gtk_widget_set_halign(argument_with_a_women_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(argument_with_a_women_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(argument_with_a_women_box, 72);
    gtk_widget_set_margin_top(argument_with_a_women_box, 0);
    gtk_widget_set_margin_bottom(argument_with_a_women_box, 0);

    gtk_widget_set_size_request(argument_with_a_women_box, 190, 254);

    // label
    gtk_widget_set_halign(argument_with_a_women_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), argument_with_a_women_label);

    gtk_widget_set_margin_top(argument_with_a_women_label, 46);

    // desc
    gtk_widget_set_halign(argument_with_a_women_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), argument_with_a_women_desc_label);

    gtk_widget_set_margin_top(argument_with_a_women_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_3, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(argument_with_a_women_box), btn_3);

    gtk_widget_set_margin_top(btn_3, 35);

    gtk_widget_set_size_request(btn_3, 50, 50);

    // second_hbox
    gtk_box_append(GTK_BOX(achievements_box), second_hbox);

    gtk_widget_set_halign(second_hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(second_hbox, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(second_hbox, 0);
    gtk_widget_set_margin_end(second_hbox, 0);
    gtk_widget_set_margin_top(second_hbox, 32);
    gtk_widget_set_margin_bottom(second_hbox, 32);

    gtk_widget_set_size_request(second_hbox, 1010, 254);

    // loving_box
    gtk_box_append(GTK_BOX(second_hbox), loving_box);

    gtk_widget_set_halign(loving_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(loving_box, 17);
    gtk_widget_set_margin_top(loving_box, 0);
    gtk_widget_set_margin_bottom(loving_box, 0);

    gtk_widget_set_size_request(loving_box, 190, 254);

    // label
    gtk_widget_set_halign(loving_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), loving_label);

    gtk_widget_set_margin_top(loving_label, 46);

    // desc
    gtk_widget_set_halign(loving_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), loving_desc_label);

    gtk_widget_set_margin_top(loving_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_4, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(loving_box), btn_4);

    gtk_widget_set_margin_top(btn_4, 35);

    gtk_widget_set_size_request(btn_4, 50, 50);

    // vicious_box
    gtk_box_append(GTK_BOX(second_hbox), vicious_box);

    gtk_widget_set_halign(vicious_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vicious_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(vicious_box, 72);
    gtk_widget_set_margin_top(vicious_box, 0);
    gtk_widget_set_margin_bottom(vicious_box, 0);

    gtk_widget_set_size_request(vicious_box, 190, 254);

    // label
    gtk_widget_set_halign(vicious_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), vicious_label);

    gtk_widget_set_margin_top(vicious_label, 46);

    // desc
    gtk_widget_set_halign(vicious_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), vicious_desc_label);

    gtk_widget_set_margin_top(vicious_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_5, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(vicious_box), btn_5);

    gtk_widget_set_margin_top(btn_5, 35);

    gtk_widget_set_size_request(btn_5, 50, 50);

    // smart_box
    gtk_box_append(GTK_BOX(second_hbox), smart_box);

    gtk_widget_set_halign(smart_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(smart_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(smart_box, 72);
    gtk_widget_set_margin_top(smart_box, 0);
    gtk_widget_set_margin_bottom(smart_box, 0);

    gtk_widget_set_size_request(smart_box, 190, 254);

    // label
    gtk_widget_set_halign(smart_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), smart_label);

    gtk_widget_set_margin_top(smart_label, 46);

    // desc
    gtk_widget_set_halign(smart_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), smart_desc_label);

    gtk_widget_set_margin_top(smart_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_6, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(smart_box), btn_6);

    gtk_widget_set_margin_top(btn_6, 35);

    gtk_widget_set_size_request(btn_6, 50, 50);

    // mission_impossible_box
    gtk_box_append(GTK_BOX(second_hbox), mission_impossible_box);

    gtk_widget_set_halign(mission_impossible_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mission_impossible_box, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_start(mission_impossible_box, 72);
    gtk_widget_set_margin_top(mission_impossible_box, 0);
    gtk_widget_set_margin_bottom(mission_impossible_box, 0);

    gtk_widget_set_size_request(mission_impossible_box, 190, 254);

    // label
    gtk_widget_set_halign(mission_impossible_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), mission_impossible_label);

    gtk_widget_set_margin_top(mission_impossible_label, 46);

    // desc
    gtk_widget_set_halign(mission_impossible_desc_label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), mission_impossible_desc_label);

    gtk_widget_set_margin_top(mission_impossible_desc_label, 35);

    // btn
    gtk_widget_set_halign(btn_7, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(mission_impossible_box), btn_7);

    gtk_widget_set_margin_top(btn_7, 35);

    gtk_widget_set_size_request(btn_7, 50, 50);

    // styling
    widget_styling(achievements_box, current_screen, "achievements_box");
    scrolled_window_styling(scrollbar, current_screen, "scrollbar");
    widget_styling(achievements_icon_btn, current_screen, "achievements_icon_btn");
    widget_styling(achievements_label_btn, current_screen, "achievements_label_btn");

    widget_styling(first_hbox, current_screen, "first_hbox");

    widget_styling(first_step_box, current_screen, "achievement_card");
    widget_styling(first_step_label, current_screen, "achievement_label_text");
    widget_styling(first_step_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.first_step)
    {
        widget_styling(btn_0, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_0, current_screen, "unfinished_btn");
    }

    widget_styling(explorer_box, current_screen, "achievement_card");
    widget_styling(explorer_label, current_screen, "achievement_label_text");
    widget_styling(explorer_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.explorer)
    {
        widget_styling(btn_1, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_1, current_screen, "unfinished_btn");
    }

    widget_styling(fickle_box, current_screen, "achievement_card");
    widget_styling(fickle_label, current_screen, "achievement_label_text");
    widget_styling(fickle_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.fickle)
    {
        widget_styling(btn_2, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_2, current_screen, "unfinished_btn");
    }

    widget_styling(argument_with_a_women_box, current_screen, "achievement_card");
    widget_styling(argument_with_a_women_label, current_screen, "achievement_label_text");
    widget_styling(argument_with_a_women_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.milka)
    {
        widget_styling(btn_3, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_3, current_screen, "unfinished_btn");
    }

    widget_styling(loving_box, current_screen, "achievement_card");
    widget_styling(loving_label, current_screen, "achievement_label_text");
    widget_styling(loving_desc_label, current_screen, "achievement_desc_text");

    if (current_achievements.loving)
    {
        widget_styling(btn_4, current_screen, "complete_btn");
    }
    else
    {
        widget_styling(btn_4, current_screen, "unfinished_btn");
    }

    widget_styling(vicious_box, current_screen, "achievement_card");
    widget_styling(vicious_label, current_screen, "achievement_label_text");
    widget_styling(vicious_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_5, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_5, current_screen, "unfinished_btn");
    //}

    widget_styling(smart_box, current_screen, "achievement_card");
    widget_styling(smart_label, current_screen, "achievement_label_text");
    widget_styling(smart_desc_label, current_screen, "achievement_desc_text");

    // if () {
    widget_styling(btn_6, current_screen, "complete_btn");
    //} else {
    // widget_styling(btn_6, current_screen, "unfinished_btn");
    //}

    widget_styling(mission_impossible_box, current_screen, "achievement_card");
    widget_styling(mission_impossible_label, current_screen, "achievement_label_text");
    widget_styling(mission_impossible_desc_label, current_screen, "achievement_desc_text");

    // if () {
    // widget_styling(btn_7, current_screen, "complete_btn");
    // } else {
    widget_styling(btn_7, current_screen, "unfinished_btn");
    //}

    widget_styling(second_hbox, current_screen, "second_hbox");
    widget_styling(loving_box, current_screen, "achievement_card");
}
