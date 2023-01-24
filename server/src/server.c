#include "header.h"

int main()
{
	int server_socket, client_socket, client_addr_size;
	struct sockaddr_in server, client;
	char message[1000], client_reply[2000];
	pid_t childpid;
	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	// Prepare the sockaddr_in structure
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(1234);

	// Bind
	if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		// print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	// Listen
	listen(server_socket, 3);

	// Accept and incoming connection
	puts("Waiting for incoming connections...");
	client_addr_size = sizeof(struct sockaddr_in);

	// accept connection from an incoming client

	// Receive a message from client
	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&client_addr_size);
		if (client_socket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		if ((childpid = fork()) == 0)
		{
			close(server_socket);
			while (recv(client_socket, client_reply, sizeof(client_reply), 0) > 0)
			{
				printf("Enter message : "); // Send the message back to client
				scanf("\n%200[0-9a-zA-Z.,! ]", message);
				if (send(client_socket, message, sizeof(message), 0) < 0)
				{
					puts("Send failed");
					return 1;
				}
				puts("client reply :");
				puts(client_reply);
			}
			if (recv(client_socket, client_reply, sizeof(client_reply), 0) == 0)
			{
				printf("Disconnected from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
				break;
			}
		}
	}
	close(client_socket);
	return 0;
}
