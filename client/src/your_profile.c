#include "../inc/client.h"

extern t_screen curent_screen;
extern t_grid curent_grid;

//static void
//on_open_response (GtkDialog *dialog, int response) {
//
//    if (response == GTK_RESPONSE_ACCEPT) {
//
//        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
//
//        g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
//
//
//    }
//
//  gtk_window_destroy (GTK_WINDOW (dialog));
//}
//
//static void choose_profile_avatar_btn_clicked(GtkWidget *box) {
//
//    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
//
//    GtkFileFilter *image_filter = gtk_file_filter_new();
//
//    gtk_file_filter_add_pattern(image_filter, "*.png");
//    gtk_file_filter_add_pattern(image_filter, "*.jpg");
//    gtk_file_filter_add_pattern(image_filter, "*.jpeg");
//    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), image_filter);
//
//    g_signal_connect (dialog, "response", G_CALLBACK (on_open_response), NULL);
//}
//
static void draw_round_mask(cairo_t *cr, double x, double y, double radius) {
   cairo_new_path(cr);
   cairo_arc(cr, x + radius, y + radius, radius, 0, 2 * M_PI);
   cairo_close_path(cr);
}

static void draw_image(GtkDrawingArea *area,
              cairo_t        *cr,
              int             width,
              int             height,
              gpointer        data) {

   cairo_surface_t *image_surface = (cairo_surface_t *)data;
   cairo_surface_t *mask_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
   draw_round_mask(cr, 0, 0, width/2);
   cairo_set_source_surface(cr, image_surface, 0, 0);
   cairo_mask_surface(cr, mask_surface, 0, 0);
   cairo_paint(cr);
}

void show_your_profile() {

   GtkWidget *profile_help_grid = create_grid(474,602, "profile_help_box");
   GtkWidget *profile_grid_for_img = create_grid(200, 200, "profile_grid_for_img");
   GtkWidget *profile_grid_scale = create_grid(251, 20, "profile_grid_scale");
   GtkWidget *profile_grid_body = create_grid(474, 385, "profile_grid_body");

   gtk_grid_attach(GTK_GRID(curent_grid.your_profile), profile_help_grid, 0, 0, 1, 1);
   gtk_grid_set_row_spacing(GTK_GRID(profile_help_grid), 10);
   gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_for_img, 0, 0, 1, 1);
   gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_scale, 0, 1, 1, 1);
   gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_body, 0, 2, 1, 1);



   GtkWidget *profile_avatar_icon = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

   GtkWidget *choose_profile_avatar_btn = gtk_button_new();

   GtkWidget *drawing_area = gtk_drawing_area_new();

   cairo_surface_t *image_surface = cairo_image_surface_create_from_png("resources/images/settings_icon_btn.png");

   GtkWidget *exp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

   GtkWidget *mini_exp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

   GtkWidget *exp_count_label = gtk_label_new("50/100 EXP");

   GtkWidget *profile_grid_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

   GtkWidget *name_label = gtk_label_new("Name");

   GtkWidget *first_name_entry = gtk_entry_new();

   gtk_entry_set_placeholder_text(GTK_ENTRY(first_name_entry), "First Name (requiered)");

   GtkWidget *last_name_entry = gtk_entry_new();

   gtk_entry_set_placeholder_text(GTK_ENTRY(last_name_entry), "Last name (optional)");

   GtkWidget *bio_label = gtk_label_new("BIO");

   GtkWidget *bio_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

   GtkWidget *bio_entry = gtk_entry_new();

   gtk_entry_set_placeholder_text(GTK_ENTRY(bio_entry), "Write something about you...");


   // if (g_signal_connect(choose_profile_avatar_btn, "clicked", NULL, NULL)) {

   // }


   gtk_drawing_area_set_content_width(GTK_DRAWING_AREA (drawing_area), 200);
   gtk_drawing_area_set_content_height(GTK_DRAWING_AREA (drawing_area), 200);
   gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (drawing_area), draw_image, image_surface, NULL);

   gtk_widget_set_halign(profile_avatar_icon, GTK_ALIGN_CENTER);

   gtk_grid_attach(GTK_GRID(profile_grid_for_img), profile_avatar_icon, 0, 0, 1, 1);

   gtk_box_append(GTK_BOX(profile_avatar_icon), drawing_area);




   gtk_widget_set_halign(choose_profile_avatar_btn, GTK_ALIGN_CENTER);

   gtk_grid_attach(GTK_GRID(profile_grid_for_img), choose_profile_avatar_btn, 0, 0, 1, 1);

   gtk_widget_set_size_request(choose_profile_avatar_btn, 200, 200);




   gtk_grid_attach(GTK_GRID(profile_grid_scale), exp_box, 0, 0, 1, 1);

   gtk_widget_set_size_request(exp_box, 250, 20);

   gtk_box_append(GTK_BOX(exp_box), mini_exp_box);

   gtk_widget_set_size_request(mini_exp_box, 125, 20);


   gtk_widget_set_halign(exp_count_label, GTK_ALIGN_CENTER);
   
   gtk_grid_attach(GTK_GRID(profile_grid_scale), exp_count_label, 0, 0, 1, 1);




   gtk_grid_attach(GTK_GRID(profile_grid_body), profile_grid_vbox, 0, 0, 1, 1);

   gtk_widget_set_size_request(profile_grid_vbox, 474, 358);



   gtk_widget_set_halign(name_label, GTK_ALIGN_CENTER);

   gtk_box_append(GTK_BOX(profile_grid_vbox), name_label);

   gtk_widget_set_margin_top(name_label, 10);



   gtk_box_append(GTK_BOX(profile_grid_vbox), first_name_entry);

   //gtk_entry_set_alignment(GTK_ENTRY(first_name_entry), 0.1);

   gtk_widget_set_margin_start(first_name_entry, 122);
   gtk_widget_set_margin_end(first_name_entry, 122);
   gtk_widget_set_margin_top(first_name_entry, 6);

   gtk_widget_set_size_request(first_name_entry, 230, 45);

   
   gtk_box_append(GTK_BOX(profile_grid_vbox), last_name_entry);

   //gtk_entry_set_alignment(GTK_ENTRY(last_name_entry), 0.1);

   gtk_widget_set_margin_start(last_name_entry, 122);
   gtk_widget_set_margin_end(last_name_entry, 122);
   gtk_widget_set_margin_top(last_name_entry, 12);

   gtk_widget_set_size_request(last_name_entry, 230, 45);


   gtk_widget_set_halign(bio_label, GTK_ALIGN_CENTER);

   gtk_box_append(GTK_BOX(profile_grid_vbox), bio_label);

   gtk_widget_set_margin_top(bio_label, 9);


   gtk_widget_set_halign(bio_box, GTK_ALIGN_CENTER);

   gtk_box_append(GTK_BOX(profile_grid_vbox), bio_box);

   gtk_widget_set_margin_top(bio_box, 9);

   gtk_widget_set_size_request(bio_box, 300, 120);


   gtk_grid_attach(GTK_GRID(profile_grid_body), bio_entry, 0, 0, 1, 1);

   gtk_widget_set_margin_start(bio_entry, 95);
   gtk_widget_set_margin_end(bio_entry, 163);
   gtk_widget_set_margin_top(bio_entry, 201);
   gtk_widget_set_margin_bottom(bio_entry, 135);

   gtk_widget_set_size_request(bio_entry, 216, 22);



















   widget_styling(choose_profile_avatar_btn, curent_screen, "choose_profile_avatar_btn");
   widget_styling(mini_exp_box, curent_screen, "mini_exp_box");
   widget_styling(exp_count_label, curent_screen, "exp_count_label");
   widget_styling(name_label, curent_screen, "name_label");
   widget_styling(first_name_entry, curent_screen, "name_entry");
   widget_styling(last_name_entry, curent_screen, "name_entry");
   widget_styling(bio_label, curent_screen, "bio_label");
   widget_styling(bio_box, curent_screen, "bio_box");
   widget_styling(bio_entry, curent_screen, "bio_entry");

}
