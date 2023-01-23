#include "header.h"

int main()
{
	int socket_desc, client_sock, client_addr_size, read_size;
	struct sockaddr_in server, client;
	char message[1000], client_reply[2000];

	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(1234);

	// Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		// print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	// Listen
	listen(socket_desc, 3);

	// Accept and incoming connection
	puts("Waiting for incoming connections...");
	client_addr_size = sizeof(struct sockaddr_in);

	// accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&client_addr_size);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");

	// Receive a message from client
	while (1)
	{
		// Send the message back to client
		printf("Enter message : ");
		scanf("\n%200[0-9a-zA-Z.,! ]", message);
		if (send(client_sock, message, sizeof(message), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		if (recv(client_sock, client_reply, sizeof(client_reply), 0) < 0)
		{
			puts("recv failed");
			break;
		}
		puts("client reply :");
		puts(client_reply);

		if ((read_size = recv(client_sock, client_reply, sizeof(client_reply), 0)) == 0)
		{
			puts("Client disconnected");
			fflush(stdout);
		}
	}
	return 0;
}
