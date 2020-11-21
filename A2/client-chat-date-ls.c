#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
#define MAX 100

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	printf("\nSocket successfully created...\n");
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}
	printf("\nSocket successfully accepted by server...\n"); 
	int n;	
	printf("1) To get the date you have to type DATE.\n2) To get the list of files you have to type ls.\n");
	while(1)
	{
	
	bzero(buffer,MAX); 
	printf("\nEnter your message for Server: ");
	n=0;
	while ((buffer[n++] = getchar()) != '\n'){}; 	
	send(sock , buffer , strlen(buffer) , 0 ); 
	bzero(buffer,MAX); 
	valread = read( sock , buffer, 1024); 
	printf("\nMessage from Server: %s",buffer );
	if(strncmp(buffer,"exit",4)==0)
	{
		printf("\nClient Exit...\n");
		break;
	} 
	}		
	return 0; 
} 





