// include "../inc/client.h"


// extern t_screen current_screen;
// extern t_grid current_grid;

// void on_open_response (GtkDialog *dialog, int response)
// {
//   if (response == GTK_RESPONSE_ACCEPT)
//     {
//       GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

//       g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);

      
//     }

//   gtk_window_destroy (GTK_WINDOW (dialog));
// }

// void choose_profile_avatar_btn_clicked_func () {
//   // ...
//   GtkWidget *dialog;
//   GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

//   dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT,NULL);

//   gtk_window_present (GTK_WINDOW (dialog));
//   GtkFileFilter *image_filter = gtk_file_filter_new();

//    gtk_file_filter_add_pattern(image_filter, "*.png");
//    gtk_file_filter_add_pattern(image_filter, "*.jpg");
//    gtk_file_filter_add_pattern(image_filter, "*.jpeg");
//    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), image_filter);


//   g_signal_connect (dialog, "response", G_CALLBACK (on_open_response),  NULL);
// }
// include "../inc/client.h"


// extern t_screen current_screen;
// extern t_grid current_grid;

// void on_open_response (GtkDialog *dialog, int response)
// {
//   if (response == GTK_RESPONSE_ACCEPT)
//     {
//       GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

//       g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);

      
//     }

//   gtk_window_destroy (GTK_WINDOW (dialog));
// }

// void choose_profile_avatar_btn_clicked_func () {
//   // ...
//   GtkWidget *dialog;
//   GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

//   dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT,NULL);

//   gtk_window_present (GTK_WINDOW (dialog));
//   GtkFileFilter *image_filter = gtk_file_filter_new();

//    gtk_file_filter_add_pattern(image_filter, "*.png");
//    gtk_file_filter_add_pattern(image_filter, "*.jpg");
//    gtk_file_filter_add_pattern(image_filter, "*.jpeg");
//    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), image_filter);


//   g_signal_connect (dialog, "response", G_CALLBACK (on_open_response),  NULL);
// }
// // // Функция для обрезки изображения
// // void crop_image(const char *filename) {
// //   // Загрузить изображение
// //   GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  
// //   // Получить размер изображения
// //   int width = gdk_pixbuf_get_width(pixbuf);
// //   int height = gdk_pixbuf_get_height(pixbuf);
  
// //   // Создать новый буфер для круглого изображения
// //   GdkPixbuf *circle_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);

// //   // Установить область рисования
// //   cairo_t *cr = gdk_cairo_create(gdk_pixbuf_get_surface(circle_buf));
  
// //   // Обрезать изображение в виде круга
// //   cairo_arc(cr, width/2.0, height/2.0, MIN(width, height)/2.0, 0, 2*G_PI);
// //   cairo_clip(cr);
  
// //   // Отрисовать изображение
// //   gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
// //   cairo_paint(cr);

// //   // Освободить ресурсы
// //   cairo_destroy(cr);
// //   g_object_unref(pixbuf);

// //   // Сохранить полученное изображение
// //   gchar *circle_filename = g_strdup_printf("%s.circle.png", filename);
// //   gdk_pixbuf_save(circle_buf, circle_filename, "png", NULL, NULL);
// //   g_object_unref(circle_buf);
// // }






// /* Функция для обрезки фотографии в виде круга */
// // void crop_image(const char *filename) {
// // // Загрузить изображение
// // GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
// // // Получить размер изображения
// // int width = gdk_pixbuf_get_width(pixbuf);
// // int height = gdk_pixbuf_get_height(pixbuf);

// // // Создать новый буфер для круглого изображения
// // GdkPixbuf *circle_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);

// // // Установить область рисования
// // cairo_t *cr = gdk_cairo_create(gdk_pixbuf_get_surface(circle_buf));
// // cairo_translate(cr, width / 2.0, height / 2.0);

// // // Обрезать изображение в виде круга
// // int radius = (MIN(width, height) / 2) - 10; // Вычисляем радиус круга
// // cairo_arc(cr, 0, 0, radius, 0, 2 * G_PI);
// // cairo_clip(cr);
// // cairo_arc(cr, 0, 0, radius, 0, 2 * G_PI);
// // cairo_set_source_rgba(cr, 1, 1, 1, 1); // установка белого цвета для заполнения обрезанной области
// // cairo_fill_preserve(cr);
// // cairo_set_source_rgba(cr, 0, 0, 0, 1); // установка черного цвета для контура обрезанной области
// // cairo_set_line_width(cr, 5.0);
// // cairo_stroke(cr);

// // // Отрисовать изображение в новом буфере
// // gdk_cairo_set_source_pixbuf(cr, pixbuf, -width / 2.0, -height / 2.0);
// // cairo_paint(cr);

// // // Освободить ресурсы
// // cairo_destroy(cr);
// // g_object_unref(pixbuf);

// // // Сохранить полученное изображение
// // gchar *circle_filename = g_strdup_printf("%s.circle.png", filename);
// // gdk_pixbuf_save(circle_buf, circle_filename, "png", NULL, NULL);
// // g_object_unref(circle_buf);
// // }
// // void on_open_response (GtkDialog *dialog, int response) {
// //   if (response == GTK_RESPONSE_ACCEPT) {
// //     GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
// //     g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
// //     gchar *filename = g_file_get_path(file);

// //     /* Загрузка изображения из файла */
// //     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    
// //     /* Обрезка изображения */
// //     crop_image(&pixbuf);

// //     /* Сохранение изображения в файл */
// //     gchar *cropped_filename = g_strdup_printf("%s.cropped.png", filename);
// //     gdk_pixbuf_save(pixbuf, cropped_filename, "png", NULL, NULL);
// //     g_free(cropped_filename);

// //     g_free(filename);
// //   }

// //   gtk_window_destroy (GTK_WINDOW (dialog));
// // }


// // void choose_profile_avatar_btn_clicked_func () {
// //   GtkWidget *dialog;
// //   GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

// //   dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT,NULL);

// //   gtk_window_present (GTK_WINDOW (dialog));

// //   // Добавить фильтр изображений
// //   GtkFileFilter *image_filter = gtk_file_filter_new();
// //   gtk_file_filter_add_pattern(image_filter, "*.png");
// //   gtk_file_filter_add_pattern(image_filter, "*.jpg");
// //   gtk_file_filter_add_pattern(image_filter, "*.jpeg");
// //   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), image_filter);

// //   g_signal_connect (dialog, "response", G_CALLBACK (on_open_response),  NULL);
// // }