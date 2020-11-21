#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <time.h>
#define PORT 8080 
#define MAX 100
#define PATH_MAX 5000
#define TRUE 1 
#define FALSE 0 
#define PORT 8080
	
int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[5] , 
		max_clients = 5 , activity, i , valread , sd,n; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1025],p[80]; 
	fd_set readfds; 
	FILE *fp;	
	
	char *message = "hello client"; 
	
	
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	

		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
						printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
				(address.sin_port)); 
		
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 
						
					break; 
				} 
			} 
		} 
			
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 

				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					getpeername(sd , (struct sockaddr*)&address , \ 
						(socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , 
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				else
				{ 
					buffer[valread] = '\0'; 
					n=0;
                    printf("Client:%d:%s",sd,buffer);
					if(strncmp(buffer,"ls",2)==0){
						bzero(buffer,MAX);
						FILE *fp;
						int status;
						char path[PATH_MAX],ch;
						fp = popen("ls *", "r");
						buffer[n++]='\n';
						while ((ch=fgetc(fp)) != EOF)
							buffer[n++]=ch;
						send(sd , buffer , strlen(buffer) , 0 );
						status = pclose(fp);
					}
					else{
						
						if((strncmp(buffer,"getfile",7))==0){
							char cmd[30]="cat ";
							strcat(cmd,buffer+8);
							bzero(buffer, sizeof(buffer)); 
							fp = popen(cmd,"r");
							int size=0;
							while (fgets(p,80,fp) != NULL){
								strcat(buffer,p);
								size+=strlen(p);
							}
							buffer[size]='\0';
							send(sd , buffer , strlen(buffer) , 0 ); 
						}
						
						else {
					
							if(strncmp(buffer,"exit",4)){
								bzero(buffer, sizeof(buffer)); 
								printf("Enter response message to client: "); 
								n=0;
								while((buffer[n++]=getchar()) != '\n')
									;
								buffer[n]='\0';
								send(sd , buffer , strlen(buffer) , 0 ); 
							}
						}
					}
				} 
			} 
		} 
	} 
		
	return 0; 
} 
