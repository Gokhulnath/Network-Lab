// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT 8080 
#define MAXLINE 1024 

//structure to store domain and its ip
typedef struct domain{
	char domain[MAXLINE];
	char ip_add[5][MAXLINE];
	int flag;
}domain_details;

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]={0}; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 
	domain_details dom_ip,result;

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len; 
	while(1)
	{
		bzero(&result, sizeof (domain_details));
		bzero(&dom_ip, sizeof (domain_details));
		printf("\nEnter your message for Server: ");
		scanf("%s", dom_ip.domain);
		if((strcmp(dom_ip.domain,"exit"))==0){
			break;
		}
		//sends the domain to the server
		sendto(sockfd, dom_ip.domain, sizeof(dom_ip.domain), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
		printf("Message sent to server.\n"); 
		//the ip address is received from the server and stored in the structure
		recvfrom(sockfd, &result, sizeof (domain_details), 0, (struct sockaddr *) &servaddr, &len);
		//checking the validity of the domain sent based on the flag response from the server
		if(result.flag==0){
			printf("\nEntry not found!!\n");
			printf("%s\n",result.ip_add[0]);
		}
		else{
			printf("Ip address: \n");
				printf("%s\n",result.ip_add[0]);
			
		}
	}
	close(sockfd); 
	return 0; 
} 
