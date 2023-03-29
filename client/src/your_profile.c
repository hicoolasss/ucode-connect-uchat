#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
t_avatar current_avatar;
t_scaled_avatar current_scaled_avatar;
t_your_profile_avatar current_your_profile_avatar;
GtkWidget *avatar_img;

static void get_your_profile_avatar()
{

  if (!current_your_profile_avatar.avatar)
  {
    g_print("Ошибка при загрузке изображения.\n");
    return;
  }

  // Масштабирование исходного изображения до размера аватара
  GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(current_your_profile_avatar.avatar, 200, 200, GDK_INTERP_BILINEAR);
  // g_object_unref(current_avatar.avatar);

  // Создание поверхности Cairo для рисования
  cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 200, 200);
  cairo_t *cr = cairo_create(surface);

  // Создание круглой области
  cairo_arc(cr, 200 / 2.0, 200 / 2.0, 200 / 2.0, 0, 2 * G_PI);
  cairo_clip(cr);
  gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
  cairo_paint(cr);

  GdkPixbuf *circle_pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 200, 200);

  current_your_profile_avatar.your_profile_avatar = circle_pixbuf;
  current_avatar.avatar = current_your_profile_avatar.avatar;

  // cairo_surface_write_to_png(surface, "avatar1.png");
}

void on_open_response(GtkDialog *dialog, int response)
{
  if (response == GTK_RESPONSE_ACCEPT)
  {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);
    gchar *filename = g_file_get_path(file);

    /* Загрузка изображения из файла */
    current_your_profile_avatar.avatar = gdk_pixbuf_new_from_file(filename, NULL);

    /* Обрезка изображения */
    get_your_profile_avatar();
    get_scaled_image();
    gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_img), current_your_profile_avatar.your_profile_avatar);

    g_free(filename);
  }

  gtk_window_destroy(GTK_WINDOW(dialog));
}

void choose_profile_avatar_btn_clicked(GtkButton *button)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

  dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

  gtk_window_present(GTK_WINDOW(dialog));

  // Добавить фильтр изображений
  GtkFileFilter *image_filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(image_filter, "*.png");
  gtk_file_filter_add_pattern(image_filter, "*.jpg");
  gtk_file_filter_add_pattern(image_filter, "*.jpeg");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), image_filter);

  g_signal_connect(dialog, "response", G_CALLBACK(on_open_response), NULL);
}

void show_your_profile()
{

  GtkWidget *profile_help_grid = create_grid(474, 602, "profile_help_box");
  GtkWidget *profile_grid_for_img = create_grid(200, 200, "profile_grid_for_img");
  GtkWidget *profile_grid_scale = create_grid(251, 20, "profile_grid_scale");
  GtkWidget *profile_grid_body = create_grid(474, 385, "profile_grid_body");

  gtk_grid_attach(GTK_GRID(current_grid.your_profile), profile_help_grid, 0, 0, 1, 1);
  gtk_grid_set_row_spacing(GTK_GRID(profile_help_grid), 10);
  gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_for_img, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_scale, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_body, 0, 2, 1, 1);

  // GtkWidget *profile_avatar_icon = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  //  get_your_profile_avatar();

  //GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/home/criops/ucode-connect-uchat/avatar.png", NULL);

  avatar_img = gtk_image_new_from_pixbuf(current_avatar.avatar);

  GtkWidget *choose_profile_avatar_btn = gtk_button_new();

  gtk_button_set_child(GTK_BUTTON(choose_profile_avatar_btn), avatar_img);

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

  // gtk_entry_set_alignment(GTK_ENTRY(first_name_entry), 0.1);

  gtk_widget_set_margin_start(first_name_entry, 122);
  gtk_widget_set_margin_end(first_name_entry, 122);
  gtk_widget_set_margin_top(first_name_entry, 6);

  gtk_widget_set_size_request(first_name_entry, 230, 45);

  gtk_box_append(GTK_BOX(profile_grid_vbox), last_name_entry);

  // gtk_entry_set_alignment(GTK_ENTRY(last_name_entry), 0.1);

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









  widget_styling(choose_profile_avatar_btn, current_screen, "choose_profile_avatar_btn");
  //widget_styling(avatar_img, current_screen, "avatar_img");
  widget_styling(mini_exp_box, current_screen, "mini_exp_box");
  widget_styling(exp_count_label, current_screen, "exp_count_label");
  widget_styling(name_label, current_screen, "name_label");
  widget_styling(first_name_entry, current_screen, "name_entry");
  widget_styling(last_name_entry, current_screen, "name_entry");
  widget_styling(bio_label, current_screen, "bio_label");
  widget_styling(bio_box, current_screen, "bio_box");
  widget_styling(bio_entry, current_screen, "bio_entry");

  g_signal_connect(choose_profile_avatar_btn, "clicked", G_CALLBACK(choose_profile_avatar_btn_clicked), NULL);
}
