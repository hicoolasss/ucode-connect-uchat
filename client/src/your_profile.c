#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
t_avatar current_avatar;
t_scaled_avatar current_scaled_avatar;
t_your_profile_avatar current_your_profile_avatar;
GtkWidget *avatar_img;

static void update_profile_pic_lmb()
{
  GtkWidget *children, *iter;

  children = gtk_widget_get_first_child(current_grid.left_menu_bar);

  for (iter = children; iter != NULL; iter = gtk_widget_get_last_child(current_grid.left_menu_bar))
  {

    gtk_widget_unparent(iter);
  }

  // current_avatar.avatar = current_your_profile_avatar.avatar;

  show_left_menu_bar();
}

void send_avatar_to_db(gchar *filepath)
{
  char *filename = basename(filepath);
  char *new_path = g_strdup_printf("resources/avatars/%s", filename);

  cJSON *json = cJSON_CreateObject();
  cJSON_AddStringToObject(json, "command", "<update_image>");
  cJSON_AddStringToObject(json, "filename", new_path);

  g_async_queue_push(message_queue, json);
}

static void get_your_profile_avatar_from_db()
{
  GdkPixbuf *source_pixbuf = gdk_pixbuf_new_from_file(current_client.avatarname, NULL);
  if (!source_pixbuf)
  {
    g_print("Ошибка при загрузке изображения.2\n");
    return;
  }

  // Масштабирование исходного изображения до размера аватара
  GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(source_pixbuf, 200, 200, GDK_INTERP_BILINEAR);
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
}

static void get_your_profile_avatar()
{

  if (!current_your_profile_avatar.avatar)
  {
    g_print("Ошибка при загрузке изображения.2\n");
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

    send_avatar_to_db(filename);
    gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_img), current_your_profile_avatar.your_profile_avatar);
    update_profile_pic_lmb();

    // g_free(new_path);
    g_free(filename);
  }

  gtk_window_destroy(GTK_WINDOW(dialog));
}

void choose_profile_avatar_btn_clicked()
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

  dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);
  gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(current_screen.screen));

  // Установить начальную папку для диалога
  const gchar *initial_folder = "resources/avatars/";
  GFile *folder = g_file_new_for_path(initial_folder);
  GError *error = NULL;
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), folder, &error);

  if (error)
  {
    g_warning("Error setting current folder: %s", error->message);
    g_error_free(error);
  }

  g_object_unref(folder);

  gtk_window_present(GTK_WINDOW(dialog));

  // Добавить фильтр изображений
  GtkFileFilter *image_filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(image_filter, "*.png");
  gtk_file_filter_add_pattern(image_filter, "*.jpg");
  gtk_file_filter_add_pattern(image_filter, "*.JPG");
  gtk_file_filter_add_pattern(image_filter, "*.jpeg");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), image_filter);

  g_signal_connect(dialog, "response", G_CALLBACK(on_open_response), NULL);
}

void show_your_profile()
{

  // GtkWidget *profile_help_grid = create_grid(474, 602, "profile_help_box");
  // GtkWidget *profile_grid_for_img = create_grid(200, 200, "profile_grid_for_img");
  // GtkWidget *profile_grid_body = create_grid(474, 385, "profile_grid_body");

 //gtk_grid_attach(GTK_GRID(current_grid.your_profile), profile_help_grid, 0, 0, 1, 1);
  // gtk_grid_set_row_spacing(GTK_GRID(profile_help_grid), 10);
  // gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_for_img, 0, 0, 1, 1);
  // gtk_grid_attach(GTK_GRID(profile_help_grid), profile_grid_body, 0, 1, 1, 1);

  gtk_widget_set_halign(current_grid.your_profile, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(current_grid.your_profile, TRUE);
    gtk_widget_set_vexpand(current_grid.your_profile, TRUE);


  get_your_profile_avatar_from_db();

  avatar_img = gtk_image_new_from_pixbuf(current_your_profile_avatar.your_profile_avatar);

  GtkWidget *choose_profile_avatar_btn = gtk_button_new();

  gtk_button_set_child(GTK_BUTTON(choose_profile_avatar_btn), avatar_img);

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




  gtk_grid_attach(GTK_GRID(current_grid.your_profile), choose_profile_avatar_btn, 0, 0, 1, 1);

  gtk_widget_set_hexpand(choose_profile_avatar_btn, TRUE);

  gtk_widget_set_halign(choose_profile_avatar_btn, GTK_ALIGN_CENTER);

  gtk_widget_set_margin_top(choose_profile_avatar_btn, 9);

  gtk_widget_set_size_request(choose_profile_avatar_btn, 200, 200);

  gtk_grid_attach(GTK_GRID(current_grid.your_profile), profile_grid_vbox, 0, 1, 1, 1);

  gtk_widget_set_margin_start(profile_grid_vbox, 268);
  gtk_widget_set_margin_end(profile_grid_vbox, 268);
  gtk_widget_set_margin_top(profile_grid_vbox, 9);
  gtk_widget_set_margin_bottom(profile_grid_vbox, 48);

  gtk_widget_set_hexpand(profile_grid_vbox, TRUE);

  gtk_widget_set_halign(profile_grid_vbox, GTK_ALIGN_CENTER);

  gtk_widget_set_size_request(profile_grid_vbox, 474, 358);
  
  gtk_widget_set_hexpand(name_label, TRUE);

  gtk_widget_set_halign(name_label, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(profile_grid_vbox), name_label);

  gtk_widget_set_margin_top(name_label, 10);

  gtk_box_append(GTK_BOX(profile_grid_vbox), first_name_entry);

  // gtk_entry_set_alignment(GTK_ENTRY(first_name_entry), 0.1);

  gtk_widget_set_halign(first_name_entry, GTK_ALIGN_CENTER);

  gtk_widget_set_margin_top(first_name_entry, 6);

  gtk_widget_set_size_request(first_name_entry, 230, 45);

  gtk_box_append(GTK_BOX(profile_grid_vbox), last_name_entry);

  // gtk_entry_set_alignment(GTK_ENTRY(last_name_entry), 0.1);

  gtk_widget_set_halign(last_name_entry, GTK_ALIGN_CENTER);

  gtk_widget_set_margin_top(last_name_entry, 12);

  gtk_widget_set_size_request(last_name_entry, 230, 45);

  gtk_widget_set_halign(bio_label, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(profile_grid_vbox), bio_label);

  gtk_widget_set_margin_top(bio_label, 9);

  gtk_widget_set_halign(bio_box, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(profile_grid_vbox), bio_box);

  gtk_widget_set_margin_start(bio_box, 87);
  gtk_widget_set_margin_end(bio_box, 87);
  gtk_widget_set_margin_top(bio_box, 9);
  gtk_widget_set_margin_bottom(bio_box, 48);

  gtk_widget_set_size_request(bio_box, 300, 120);

  gtk_box_append(GTK_BOX(bio_box), bio_entry);

  gtk_widget_set_margin_start(bio_entry, 7);
  gtk_widget_set_margin_end(bio_entry, 77);
  gtk_widget_set_margin_top(bio_entry, 11);
  gtk_widget_set_margin_bottom(bio_entry, 87);

  gtk_widget_set_size_request(bio_entry, 216, 22);

  widget_styling(choose_profile_avatar_btn, current_screen, "choose_profile_avatar_btn");

  widget_styling(profile_grid_vbox, current_screen, "profile_grid_body");
  // widget_styling(avatar_img, current_screen, "avatar_img");
  // widget_styling(mini_exp_box, current_screen, "mini_exp_box");
  // widget_styling(exp_count_label, current_screen, "exp_count_label");
  widget_styling(name_label, current_screen, "name_label");
  widget_styling(first_name_entry, current_screen, "name_entry");
  widget_styling(last_name_entry, current_screen, "name_entry");
  widget_styling(bio_label, current_screen, "bio_label");
  widget_styling(bio_box, current_screen, "bio_box");
  widget_styling(bio_entry, current_screen, "bio_entry");

  g_signal_connect(choose_profile_avatar_btn, "clicked", G_CALLBACK(choose_profile_avatar_btn_clicked), NULL);
}
