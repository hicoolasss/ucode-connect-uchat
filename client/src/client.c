#include "header.h"
int main(int argc, char *argv[])
{
	if(argc != 2) {
		puts("Write ip adress of server");
		exit(1);
	}
	int sock;
	struct sockaddr_in client;
	char message[1000] , server_reply[2000];
	const char *ip = argv[1];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	client.sin_addr.s_addr = inet_addr(ip);
	client.sin_family = AF_INET;
	client.sin_port = htons(1234);

	//Connect to remote client
	if (connect(sock , (struct sockaddr *)&client , sizeof(client)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
	//keep communicating with client
	while(1)
	{
		printf("Enter message : ");
		scanf("\n%200[0-9a-zA-Z.,! ]" , message);
		
		//Send some data
		if( send(sock , message , sizeof(message), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		//Receive a reply from the client
		if( recv(sock , server_reply , sizeof(server_reply) , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		puts("client reply :");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}
