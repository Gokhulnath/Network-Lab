#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT	 8080 
#define MAXLINE 1024 

//structure to store domain and its ip
typedef struct domain{
	char domain[MAXLINE];
	char ip_add[5][MAXLINE];
	int flag;
}domain_details;

//The domain name extraction
void split_func(char str[],char root_ans[])
{
	int len=strlen(str);
	int count=0;
	char arr_tokens[3][MAXLINE];
	char tld_ans[MAXLINE];
	char *token = strtok(str,".");
	while(token!=NULL ) 
	{
		//printf("token %d: %s\n",count,token); 
		strcpy(arr_tokens[count],token);
		token = strtok(NULL,".");
		count++;
	}
	strcpy(tld_ans,arr_tokens[1]);
	strcpy(root_ans,arr_tokens[2]);
	printf("Root ans: %s",root_ans);
	printf("TLD ans: %s",tld_ans);
}


int main() { 
	int sockfd; 
	char buffer[100]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr;
	domain_details dom_ip[10],*result;
	int domain_no=2;// total number of domain, its 2 cause of hard coded value
	
	// hard coding the domain and its ip, flag 1 denotes valid and 0 is invalid
	strcpy(dom_ip[0].domain,"www.google.com");
	strcpy(dom_ip[0].ip_add[0],"192.2.40.55");
	dom_ip[0].flag=1;
	
	strcpy(dom_ip[1].domain,"www.youtube.com");
	strcpy(dom_ip[1].ip_add[0],"190.24.60.75");
	dom_ip[1].flag=1;
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("\nSocket successfully created...\n");
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("\nSocket successfully binded...\n"); 
	
	int len, n; 

	len = sizeof(cliaddr); //len is value/resuslt 

	
	while(1){
		int a=0;
		//displays all domains and its ip
		printf("\nDomains list\n");
		while(a!=domain_no){
			printf("\n%s\n",dom_ip[a].domain);
			printf("%s\n",dom_ip[a].ip_add[0]);
			a++;
		}
		//recives the domain name
		n = recvfrom(sockfd,buffer, sizeof (buffer), 0, ( struct sockaddr *) &cliaddr, &len); 
		printf("\nDatagram packet arrived\n"); 
		printf("%s\n",buffer);
		a=0;
		int msg=-1;
		//validates the domain name
		while(a!=domain_no)
		{
			if(strcmp(buffer,dom_ip[a].domain)==0){
				msg=a;
				break;
			}
			a++;
		}
		//sends the ip address to the client as structure
		if(msg!=-1){
			result=&dom_ip[msg];
			sendto(sockfd, result, sizeof(domain_details), 0, (const struct sockaddr *) &cliaddr, len); 
		}
		else{
			dom_ip[msg].flag=0;
			result=&dom_ip[msg];
			sendto(sockfd, result, sizeof(domain_details), 0, (const struct sockaddr *) &cliaddr, len); 
		}
		printf("Message sent to client.\n"); 
	}
	close(sockfd);
	return 0; 
} 
