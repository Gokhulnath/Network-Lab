#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
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
	int len, clients[5], selsd;
	int sockfd, newfd, n = 0;
	struct sockaddr_in servaddr, cliaddr;

	fd_set readfds;

	char buff[1024];
	char str[1000];

	char source_ip[30],source_mac[30],destination_ip[30];

	strcpy(source_ip,"192.168.0.1");
	strcpy(source_mac,"01:a2:3c:4d:5e:6f");
	strcpy(destination_ip,"192.168.1.2");
	char temp_ip1[30],temp_ip2[30];
	strcpy(temp_ip1,"192.168.0.1");
	strcpy(temp_ip2,"192.168.1.2");

	printf("IP Address: %s\nMAC Address: %s\n",source_ip,source_mac);
	for(int i = 0; i < 5; i++) 
		clients[i] = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Server Socket is created.\n");

	if(sockfd < 0)
		perror("Cannot create socket!\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	if((bind(sockfd,(SA*)&servaddr,sizeof(servaddr)))!=0)
    {
        printf("Socket bind failed..\n");
        exit(0);
    }
    printf("Socket successfully binded..\n");
	
	if(listen(sockfd, 5) == 0){
		printf("Listening....\n");
	}else{
		printf("Error in binding.\n");
	}
	len = sizeof(cliaddr);
	int all_clients_open =1;
    int total_connected=0;

	while(all_clients_open==1) {
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		selsd = sockfd;
		for(int i = 0; i < 5; i++) {
			int sd = clients[i];
			if(sd > 0) FD_SET(sd, &readfds);
			if(sd > selsd) selsd = sd;
		}

		int activity = select(selsd + 1, &readfds, NULL, NULL, NULL);
		
		if(FD_ISSET(sockfd, &readfds)) {
			newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
			for(int i = 0; i < 5; i++) {
				if(clients[i] == 0) {
					clients[i] = newfd;
					printf("New client %d connected\n",i);
                    total_connected++;
					break;
				}
			}
		}
        if(total_connected==2)
        {
            total_connected=0;
            printf("Broadcasting message to clients.\n");
			ARP temp;
			strcpy(temp.source_ip,source_ip);
			strcpy(temp.source_mac,source_mac);
			strcpy(temp.destination_ip,destination_ip);
			strcpy(temp.destination_mac,"");
			temp.flag=0;
			printf("Broadcasting ARP Request Packet as follows:-\n");
			printf("\tSource IP address: %s\n\tSource MAC address: %s\n",temp.source_ip,temp.source_mac);
			printf("\tDestination IP address: %s\n\tDestination MAC address: %s\n",temp.destination_ip,temp.destination_mac);
			printf("\tRequest or Reply bit: %d\n",temp.flag);
            for(int i=0;i<5;i++)
            {
                if(clients[i]!=0)//That is its connected
                {
					send(clients[i],&temp,sizeof(temp),0);
                }
            }
        }
        for(int i = 0; i < 5; i++) {
			if(FD_ISSET(clients[i], &readfds))
			{
                ARP temp;
                int val_read;
                if(val_read=recv(clients[i],&temp,sizeof(temp),0)!=0)
                {
                    printf("ARP recivied from Client %d\n",(i));
                    printf("The Details are:-\n");
					printf("\tDestination IP Address: %s\n\tDestination MAC Address: %s\n\tRequest/Reply Bit: %d\n",temp.destination_ip,temp.destination_mac,temp.flag);
					all_clients_open=0;
                    break;
                }

			}

		}

	}
	printf("All clients are closed.. Server shutting down..\n");
	close(sockfd);
	return 0;
}

/*
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop/New folder (2)$ gcc server.c -o s
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop/New folder (2)$ ./s
IP Address: 192.168.0.1
MAC Address: 01:a2:3c:4d:5e:6f
Server Socket is created.
Socket successfully binded..
Listening....
New client 0 connected
New client 1 connected
Broadcasting message to clients.
Broadcasting ARP Request Packet as follows:-
Source IP address: 192.168.0.1
Source MAC address: 01:a2:3c:4d:5e:6f
Destination IP address: 192.168.0.2
Destination MAC address:a1:ae:44:5f:2d:1a
Request or Reply bit: 0
ARP recivied from Client 2
The Details are:-
Destination IP Address: 192.168.2.2
Destination MAC Address: 22:a1:b3:f4:d5:a6
Request/Reply Bit: 1
All clients are closed.. Server shutting down..
*/