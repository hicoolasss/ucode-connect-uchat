#include "server.h"
#include <gtk/gtk.h>

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


static void
app_activate(GApplication *app)
{
	GtkWidget *win, *box, *username, *password;

	win = gtk_application_window_new(GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(win), "darkchat");
	gtk_window_set_default_size(GTK_WINDOW(win), 1200, 760);

	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider,
									"client/style/theme.css");

	GtkStyleContext *context = gtk_widget_get_style_context(win);
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *welcome = gtk_label_new_with_mnemonic ("Welcome to the dark");

	GtkWidget *login_button = gtk_button_new_with_mnemonic("Login");

	GtkWidget *dont_have_account = gtk_label_new_with_mnemonic("Don’t have an account?");

	GtkWidget *signup = gtk_button_new();

	//GtkWidget *signup_label = gtk_label_new_with_mnemonic("Sign up");


	gtk_button_set_label(GTK_BUTTON(signup), "Sign up");

	gtk_button_set_has_frame(GTK_BUTTON(signup), FALSE);


	gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

	gtk_widget_set_margin_start(box, 307);
	gtk_widget_set_margin_end(box, 307);
	gtk_widget_set_margin_top(box, 108);
	gtk_widget_set_margin_bottom(box, 108);

	gtk_widget_set_size_request(box, 586, 544);

	gtk_window_set_child(GTK_WINDOW(win), box);

	username = gtk_entry_new();
	password = gtk_entry_new();

	gtk_entry_set_alignment(GTK_ENTRY(username), 0.1);
	gtk_entry_set_alignment(GTK_ENTRY(password), 0.1);
	gtk_entry_set_placeholder_text(GTK_ENTRY(username), "Username");
	gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");
	

	gtk_box_append(GTK_BOX(box), welcome);
	gtk_box_append(GTK_BOX(box), username);
	gtk_box_append(GTK_BOX(box), password);
	gtk_box_append(GTK_BOX(box), login_button);
	gtk_box_append(GTK_BOX(box), dont_have_account);
	gtk_box_append(GTK_BOX(box), signup);

	gtk_widget_set_margin_start(welcome, 103);
	gtk_widget_set_margin_end(welcome, 99);
	gtk_widget_set_margin_top(welcome, 60);

	gtk_widget_set_size_request(welcome, 384, 35);

	gtk_widget_set_margin_start(username, 81);
	gtk_widget_set_margin_end(username, 81);
	gtk_widget_set_margin_top(username, 48);

	gtk_widget_set_size_request(username, 423, 63);

	gtk_widget_set_margin_start(password, 81);
	gtk_widget_set_margin_end(password, 81);
	gtk_widget_set_margin_top(password, 25);

	gtk_widget_set_size_request(password, 423, 63);

	gtk_entry_set_visibility(GTK_ENTRY(password),FALSE);

	gtk_widget_set_margin_start(login_button, 81);
	gtk_widget_set_margin_end(login_button, 81);
	gtk_widget_set_margin_top(login_button, 37);

	gtk_widget_set_size_request(login_button, 423, 53);

	gtk_widget_set_margin_start(dont_have_account, 163);
	gtk_widget_set_margin_end(dont_have_account, 163);
	gtk_widget_set_margin_top(dont_have_account, 25);

	gtk_widget_set_size_request(dont_have_account, 260, 23);

	gtk_widget_set_margin_start(signup, 261);
	gtk_widget_set_margin_end(signup, 262);
	gtk_widget_set_margin_top(signup, 0);

	gtk_widget_set_size_request(signup, 63, 20);


	context = gtk_widget_get_style_context(box);

	gtk_style_context_add_class(context, "box");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	context = gtk_widget_get_style_context(welcome);

	gtk_style_context_add_class(context, "welcome_to_the_dark");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


	context = gtk_widget_get_style_context(username);

	gtk_style_context_add_class(context, "login");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	context = gtk_widget_get_style_context(password);

	gtk_style_context_add_class(context, "login");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	context = gtk_widget_get_style_context(login_button);

	gtk_style_context_add_class(context, "login_button");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	context = gtk_widget_get_style_context(dont_have_account);

	gtk_style_context_add_class(context, "dont_have_account");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	context = gtk_widget_get_style_context(signup);

	gtk_style_context_add_class(context, "signup");
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_widget_show(win);
}

void login_page()
{
	// GtkWidget *box, *button;

	// GtkCssProvider *provider = gtk_css_provider_new();
	// gtk_css_provider_load_from_path(provider,
	// 								"/home/criops/test_uchat/ucode-connect-uchat/client/style/theme.css");

	// if (box)
	// {

	// // 	GtkStyleContext *context = gtk_widget_get_style_context(box);

	// // 	gtk_style_context_add_class(context, "box");
	// // 	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	// }

	// username = gtk_entry_new();
	// password = gtk_entry_new();

	// gtk_entry_set_placeholder_text(GTK_ENTRY(username), "Username");
	// gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");

	// GtkStyleContext *context = gtk_widget_get_style_context(username);

	// gtk_style_context_add_class(context, "entry_login");
	// gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
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

	printf("Please enter your name: ");
	fgets(name, 32, stdin);
	str_trim_lf(name, strlen(name));

	if (strlen(name) > 32 || strlen(name) < 2)
	{
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

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
	app = gtk_application_new("com.github.ToshioCP.lb1", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	stat = g_application_run(G_APPLICATION(app), FALSE, NULL);
	// login_page();

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
