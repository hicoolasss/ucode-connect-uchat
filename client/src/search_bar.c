#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;


void search_btn_clicked_chat() {
    const char *text;

    text = gtk_editable_get_text (GTK_EDITABLE (current_grid.entry_for_search));
    call_new_chat_and_add_iter(text);
    gtk_entry_set_activates_default(GTK_ENTRY(current_grid.entry_for_search), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_grid.entry_for_search), "Search");
}

void search_btn_clicked_group() {
    const char *text;

    text = gtk_editable_get_text (GTK_EDITABLE (current_grid.entry_for_search));
    call_new_group_and_add_iter(text);
    gtk_entry_set_activates_default(GTK_ENTRY(current_grid.entry_for_search), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_grid.entry_for_search), "Search");
}

void show_search_bar() {

    GtkWidget *search_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    current_grid.search_btn = gtk_button_new();
    current_grid.entry_for_search = gtk_entry_new();
    gtk_entry_set_activates_default(current_grid.entry_for_search, FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(current_grid.entry_for_search), "Search");

    gtk_widget_set_size_request(search_bar_box, 391, 24);
    gtk_widget_set_size_request(current_grid.entry_for_search, 357, 21);
    gtk_widget_set_size_request(current_grid.search_btn, 24, 24);


    widget_styling(search_bar_box, current_screen, "search_bar_box");
    widget_styling(current_grid.search_btn, current_screen, "search_btn");
    widget_styling(current_grid.entry_for_search, current_screen, "entry_for_search");

    gtk_box_append(GTK_BOX(search_bar_box), current_grid.search_btn);
    gtk_box_append(GTK_BOX(search_bar_box), current_grid.entry_for_search);

//    if(current_grid.chat_search){
//        g_signal_connect(current_grid.search_btn, "clicked", G_CALLBACK(search_btn_clicked_chat), NULL);
//        current_grid.chat_search = FALSE;
//    } else if(current_grid.group_search){
//        g_signal_connect(current_grid.search_btn, "clicked", G_CALLBACK(search_btn_clicked_group), NULL);
//        current_grid.group_search = FALSE;
//    }
    gtk_grid_attach(GTK_GRID(current_grid.search_bar), search_bar_box, 0, 0, 1, 1);
}
