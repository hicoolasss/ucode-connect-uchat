#include "../inc/client.h"

t_screen curent_screen;
t_main main_client;
t_client cur_client;
t_grid curent_grid;
pthread_mutex_t cl_mutex;
_Atomic bool registered;
int send_all(SSL *sockfd, char *buf, int len);
int recv_all(SSL *sockfd, char *buf, int len);

void loadstyles()
{

    curent_screen.provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(curent_screen.provider,
                                    "resources/style/theme.css");
}

void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class)
{

    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, name_of_css_class);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(screen.provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void app_activate(GApplication *app)
{

    curent_screen.screen = gtk_application_window_new(GTK_APPLICATION(app));

    gtk_window_set_title(GTK_WINDOW(curent_screen.screen), "darkchat");
    gtk_window_set_default_size(GTK_WINDOW(curent_screen.screen), 1200, 760);
    // load default colorscheme
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
    cur_client.ssl = ssl;
    cur_client.serv_fd = server_fd;
    GtkApplication *app;
    int stat;

    app = gtk_application_new("com.github.darkchat", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION(app), FALSE, NULL);

    char *json_str;
    char buffer[2048];

    json_str = registration();
    send_message_to_server(json_str);

    while (main_client.connected == false)
    {
        int len = SSL_read(cur_client.ssl, buffer, sizeof(buffer) - 1);
        if (len == -1)
        {
            printf("Error receiving message\n");
        }
        if (mx_strcmp(buffer, "success\n") == 0)
        {
            main_client.connected = true;
            break;
        }
        else {
            json_str = registration();
            send_message_to_server(json_str);
        }
    }

    pthread_t rec_th;
    pthread_mutex_init(&cl_mutex, NULL);
    pthread_create(&rec_th, NULL, recv_func, &cur_client.serv_fd);
    
    while (main_client.connected == true)
    {
        mx_printstr("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        json_str = convert_to_json(buffer);
        if (json_str != NULL)
            send_message_to_server(json_str);
        memset(json_str, 0, mx_strlen(json_str));
        memset(buffer, 0, sizeof(buffer));
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(server_fd);
    pthread_exit(NULL);
    return stat;
}

int send_message_to_server(char *buffer)
{
    int len = send_all(cur_client.ssl, buffer, mx_strlen(buffer));

    if (len < 0)
    {
        mx_printstr("Error sending message.\n");
        return 1;
    }
    memset(buffer, 0, mx_strlen(buffer));
    return 0;
}

char *convert_to_json(char *buffer)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", cur_client.login);
    // cJSON_AddStringToObject(json, "password", cur_client.passwd);
    cJSON_AddStringToObject(json, "message", buffer);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
}

char *registration()
{
    char buffer[2048];
    mx_printstr("Enter your name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cur_client.login = mx_strdup(buffer);
    memset(buffer, 0, sizeof(buffer));

    mx_printstr("Enter your password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cur_client.passwd = mx_strdup(buffer);
    memset(buffer, 0, sizeof(buffer));

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "login", cur_client.login);
    cJSON_AddStringToObject(json, "password", cur_client.passwd);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return (json_str);
}

int send_all(SSL *sockfd, char *buf, int len)
{
    ssize_t n;

    while (len > 0)
    {
        n = SSL_write(sockfd, buf, len);
        if (n <= 0)
            return -1;
        buf += n;
        len -= n;
    }

    return 1;
}

int recv_all(SSL *sockfd, char *buf, int len)
{

    ssize_t n;

    while (len > 0)
    {
        n = SSL_read(sockfd, buf, len);
        if (n <= 0)
            return -1;
        buf += n;
        len -= n;
    }

    return 1;
}
