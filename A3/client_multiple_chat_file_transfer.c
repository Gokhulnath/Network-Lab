//Client Program for chat
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 1000 
#define PORT 8080 
#define SA struct sockaddr 
int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli;
	char buff[MAX]; 
	int n; 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT);
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n");
	
	printf("\nYou can chat to server\n1.ls (to get the list of files)\n2.getfile <filename> (to get the specified file)\n3.exit (to exit)\n");
	while(1){ 
		bzero(buff, sizeof(buff)); 
		printf("\nEnter Message or command to server: "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			close(sockfd);
			break;
		} 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
	}
} 
