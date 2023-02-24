#include "../inc/client.h"

t_screen curent_screen;
t_main main_client;
t_client cur_client;

void loadstyles() {

    curent_screen.provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class) {
	
	GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, name_of_css_class);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void app_activate(GApplication *app)
{

    curent_screen.screen = gtk_application_window_new(GTK_APPLICATION(app));

    gtk_window_set_title(GTK_WINDOW(curent_screen.screen), "darkchat");
    gtk_window_set_default_size(GTK_WINDOW(curent_screen.screen), 1200, 760);

    loadstyles();

    widget_styling(curent_screen.screen, curent_screen, "background");

    show_auth();

    gtk_window_present(GTK_WINDOW(curent_screen.screen));
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
	cur_client.ssl = ssl;
	GtkApplication *app;
    int stat;
    app = gtk_application_new("com.github.darkchat", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION(app), FALSE, NULL);

    // char buffer[1024];
    // while (1)
    // {
    //     mx_printstr("Enter message: ");
    //     fgets(buffer, sizeof(buffer), stdin);

    //     int len = SSL_write(cur_client.ssl, buffer, mx_strlen(buffer));

    //     if (len < 0)
    //     {
    //         mx_printstr("Error sending message.\n");
    //         break;
    //     }
    //     memset(buffer, 0, sizeof(buffer));

    // }

    SSL_shutdown(cur_client.ssl);
    SSL_free(cur_client.ssl);
    SSL_CTX_free(ctx);
    close(server_fd);
    return 0;
}
