#include "../inc/server.h"
#include"../inc/head_db.h"

t_list *users_list;
pthread_mutex_t clients_mutex;
_Atomic unsigned int cli_count;

int main(int argc, char **argv) {
    if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	int port = atoi(argv[1]);
    
    struct sockaddr_in server_addr;
    struct sockaddr_in cli_addr;

    void daemon_server();

    SSL_CTX *ctx = SSL_STX_Init();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind to address");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Failed to listen on socket");
        exit(EXIT_FAILURE);
    }
    socklen_t adr_size = sizeof(cli_addr);
    while (1) {
        // char login[10];
        // char password [10];
        SSL *ssl;

        int client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &adr_size);

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);
        SSL_accept(ssl);

        pthread_t thread;
        pthread_mutex_init(&clients_mutex, NULL);
        t_client *new_client = create_new_client(cli_addr, client_fd, ssl);
        printf("New client connected. His id: %d\n", new_client->id);

    //    ent_sys(login,password,ssl);//registration and enter db


        // sql_create_db();

        // printf("Enter login...\n");
        // scanf("%10s[^\n]", login);
        // printf("Enter pass...\n");
        // scanf("%10s[^\n]", password);
        // //"*" - поля таблицы которые хотим получить
        // char *log_id_check = sql_get("id","login",login,1);
        // char *pass_id_check = sql_get("id","password",password,1);

        // if((log_id_check != NULL) && (pass_id_check != NULL)){
        //     int log_check = mx_atoi(log_id_check);
        //     int pass_check = mx_atoi(pass_id_check);
        //     if(log_check == pass_check){printf("You are in chat(entered)!\n");}
        // }else if((log_id_check != NULL) &&(pass_id_check == NULL))
        // {
        //     printf("incorrect password...\n");}
        // else{
        //     sql_set_user(login,password,ssl);
        //     printf("You was regestered :-)\n");
        // }
        

        pthread_create(&thread, NULL, handle_client, new_client);
        pthread_detach(thread);
    }
    SSL_CTX_free(ctx);
    close(server_fd);
    pthread_exit(NULL);
    return 0;
}


