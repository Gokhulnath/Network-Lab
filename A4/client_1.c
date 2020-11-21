#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define PORT 8080
#define MAX 1024
#define SA struct sockaddr

typedef struct{
    char source_ip[30];
    char source_mac[30];
    char destination_ip[30];
    char destination_mac[30];
    int flag;
}ARP;


int main()
{
	int len;
	int sockfd, n = 0;
	struct sockaddr_in servaddr, cliaddr;
    char source_ip[30],source_mac[30];

	strcpy(source_ip,"192.168.0.2");
	strcpy(source_mac,"a1:ae:44:5f:2d:1a");
	char temp_ip[30];
	strcpy(temp_ip,"192.168.0.2");
	
    printf("IP Address: %s\nMAC Address: %s\n",source_ip,source_mac);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("socket creation falied..\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully created.\n");
    }

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

    if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr))!=0)
    {
        printf("Connection with the server failed..\n");
        exit(0);
    }
    else
    {
        printf("Connected to the server..\n");
    }
	printf("Ready to recive data.\n");
    ARP new_temp;
    recv(sockfd,&new_temp,sizeof(new_temp),0);
    printf("Data recived from server.\n");
    if(strcmp(source_ip,new_temp.destination_ip)==0)
    {
        printf("Server destinationination found sending destinationination mac address.\n");
        new_temp.flag=1;
        strcpy(new_temp.destination_mac,source_mac);
        send(sockfd,&new_temp,sizeof(new_temp),0);
        printf("Updated Data with Mac address sent to server.\n");
    }
    printf("Closing CLient.\n");
	close(sockfd);
	return 0;
}

/*
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop/New folder (2)$ gcc client_1.c -o c1
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop/New folder (2)$ ./c1
IP Address: 192.168.0.2
MAC Address: a1:ae:44:5f:2d:1a
Socket successfully created.
Connected to the server..
Ready to recive data.
Data recived from server.
Closing CLient.

*/