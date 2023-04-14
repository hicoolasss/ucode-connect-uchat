#include "../inc/client.h"
extern t_list *user_list;
t_screen current_screen;
t_main main_client;
t_client current_client;
t_grid current_grid;
pthread_mutex_t cl_mutex;
int in_chat = 0;
_Atomic bool registered;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t new_data_cond = PTHREAD_COND_INITIALIZER;
int send_all(SSL *sockfd, char *buf, int len);
int recv_all(SSL *sockfd, char *buf, int len);

// void load_custom_font(const char* font_path, GtkWidget* widget) {
//     PangoFontMap* font_map = pango_cairo_font_map_get_default();
//     PangoFontDescription* font_description = pango_font_description_new();
//     char* font_name = g_path_get_basename(font_path);

//     pango_font_description_set_family(font_description, font_name);
//     pango_font_description_set_weight(font_description, PANGO_WEIGHT_NORMAL);
//     pango_font_description_set_style(font_description, PANGO_STYLE_NORMAL);

//     PangoContext* context = gtk_widget_create_pango_context(widget);
//     const char* font_name = "JetBrainsMono";
//     PangoFontFamily* font_family = pango_font_family_new_from_name(font_name);
//     pango_font_map_load_font(font_map, context, font_description);

//     g_object_unref(context);
//     g_object_unref(font_map);
//     pango_font_description_free(font_description);
//     g_free(font_name);
// }

void loadstyles()
{

    current_screen.provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(current_screen.provider,
                                    "resources/style/theme.css");
}

void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class)
{

    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, name_of_css_class);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void widget_restyling(GtkWidget *widget, t_screen screen, const char *name_of_removed_css_class, const char *name_of_css_class)
{

    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_remove_class(context, name_of_removed_css_class);
    gtk_style_context_add_class(context, name_of_css_class);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void app_activate(GApplication *app)
{

    current_screen.screen = gtk_application_window_new(GTK_APPLICATION(app));

    gtk_window_set_title(GTK_WINDOW(current_screen.screen), "darkchat");
    gtk_window_set_default_size(GTK_WINDOW(current_screen.screen), 1200, 760);
    // gtk_window_set_resizable(GTK_WINDOW(current_screen.screen), TRUE);
    //  load default colorscheme
    change_scheme_to_any_color("#171722",
                               "#212130",
                               "#B1BAE6",
                               "#7AA2F7",
                               "#434461",
                               "#434461",
                               "#6E78A8",
                               "#13131A",
                               "#565F89",
                               "#565F89",
                               "#565F89",
                               "#0269A4",
                               "#13131A",
                               "#171722",
                               "rgba(122, 162, 247, 0.35)",
                               "#4A4A66",
                               "#7AA2F7",
                               "rgba(122, 162, 247, 0.5)",
                               "#212130",
                               "#434461",
                               "#13131A");

    // load_custom_font("../../resources/fonts/JetBrains_Mono/static/JetBrainsMono-Regular.ttf");
    // load_custom_font("../../resources/fonts/Inter/static/Inter-Regular.ttf");
    loadstyles();

    widget_styling(current_screen.screen, current_screen, "background");

    show_auth();

    gtk_window_present(GTK_WINDOW(current_screen.screen));
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    SSL_CTX *ctx = CTX_initialize_client();
    SSL *ssl;
    struct sockaddr_in server_addr;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    main_client.context = ctx;
    main_client.connected = false;
    registered = false;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server_fd);

    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    current_client.ssl = ssl;
    current_client.serv_fd = server_fd;
    GtkApplication *app;
    int stat = 0;

    pthread_t rec_th;
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_create(&rec_th, NULL, recv_func, &current_client.serv_fd);

    app = gtk_application_new("com.github.darkchat", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION(app), FALSE, NULL);

    pthread_cancel(rec_th);
    pthread_join(rec_th, NULL);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(server_fd);

    while (friend_list != NULL)
    {
        t_list *tmp = friend_list;
        friend_list = friend_list->next;
        free(tmp->data);
        free(tmp);
    }

    while (user_list != NULL)
    {
        t_list *tmp = user_list;
        user_list = user_list->next;
        free(tmp->data);
        free(tmp);
    }
    return stat;
}
