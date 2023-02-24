#include "../inc/client.h"

 t_screen curent_screen;

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[32];

void str_overwrite_stdout()
{
	printf("%s", "> ");
	fflush(stdout);
}

void str_trim_lf(char *arr, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{ // trim \n
		if (arr[i] == '\n')
		{
			arr[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig)
{
	if (sig == 2)
		flag = 1;
}

void *send_msg_handler()
{
	char message[LENGTH];
	char buffer[LENGTH + 32];

	while (1)
	{
		str_overwrite_stdout();
		fgets(message, LENGTH, stdin);
		str_trim_lf(message, LENGTH);

		if (strcmp(message, "exit") == 0)
		{
			break;
		}
		else
		{
			sprintf(buffer, "%s: %s\n", name, message);
			send(sockfd, buffer, strlen(buffer), 0);
		}

		// bzero(message, LENGTH);
		// bzero(buffer, LENGTH + 32);
	}
	catch_ctrl_c_and_exit(2);
	return NULL;
}

void *recv_msg_handler()
{
	char message[LENGTH];
	while (1)
	{
		int receive = recv(sockfd, message, LENGTH, 0);
		if (receive > 0)
		{
			printf("%s", message);
			str_overwrite_stdout();
		}
		else if (receive == 0)
		{
			break;
		}
		else
		{
			// -1
		}
		memset(message, 0, sizeof(message));
	}
	return NULL;
}


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

    //load default colorscheme
    change_scheme_to_any_color("#0ff");

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

	signal(SIGINT, catch_ctrl_c_and_exit);

	// printf("Please enter your name: ");
	// fgets(name, 32, stdin);
	// str_trim_lf(name, strlen(name));

	// if (strlen(name) > 32 || strlen(name) < 2)
	// {
	// 	printf("Name must be less than 30 and more than 2 characters.\n");
	// 	return EXIT_FAILURE;
	// }

	struct sockaddr_in server_addr;

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	// Connect to Server
	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1)
	{
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}
	/////////////////////////////////////////////////////////////////////////////////////

    GtkApplication *app;
    int stat;
    app = gtk_application_new("com.github.darkchat", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION(app), FALSE, NULL);

	/////////////////////////////////////////////////////////////////////////////////////
	// Send name
	send(sockfd, name, 32, 0);

	printf("=== WELCOME TO THE CHATROOM ===\n");

	pthread_t send_msg_thread;
	if (pthread_create(&send_msg_thread, NULL, send_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
	if (pthread_create(&recv_msg_thread, NULL, recv_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	while (1)
	{
		if (flag)
		{
			printf("\nBye\n");
			break;
		}
	}

	close(sockfd);

	return stat;
}
