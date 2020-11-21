#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h>
#define PORT 8080 
#define MAX 100
#define PATH_MAX 500
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("\nSocket successfully created...\n");
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	printf("\nSocket successfully binded...\n"); 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}
	printf("\nSocket successfully accepted the client...\n"); 	
	int n;
	while(1)
	{	bzero(buffer,MAX);
		n=0;
		valread = read( new_socket , buffer, 1024);
		printf("\nMessage from Client: %s",buffer); 
	if(strncmp(buffer,"DATE",4)==0){
		bzero(buffer,MAX);
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		snprintf(buffer,sizeof(buffer),"%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		send(new_socket , buffer , strlen(buffer) , 0 );
	}
	else{
		if(strncmp(buffer,"ls",2)==0){
			bzero(buffer,MAX);
			FILE *fp;
			int status;
			char path[PATH_MAX],ch;
			fp = popen("ls *", "r");
			buffer[n++]='\n';
			while ((ch=fgetc(fp)) != EOF)
				buffer[n++]=ch;
			send(new_socket , buffer , strlen(buffer) , 0 );
			status = pclose(fp);
		}
		else{
			bzero(buffer,MAX);
			printf("\nEnter your mesage for Client: ");
			while ((buffer[n++] = getchar()) != '\n'){}; 	
			send(new_socket , buffer , strlen(buffer) , 0 );
				if(strncmp(buffer,"exit",4)==0)
				{
					printf("\nServer Exit...\n");
					break;
				}
			}
		}	
	}	
	return 0; 
} 







