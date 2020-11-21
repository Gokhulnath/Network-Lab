#include <stdio.h>
#include<ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 500
#define PORT 8081
#define SA struct sockaddr 
int main()
{
	int sockfd, connfd, len,alp=0,num=0,vow=0,other=0;
	struct sockaddr_in servaddr, cli;
	char buff[MAX];
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) !=0)
	{
		printf("socket bind failed...\n");
		exit(0); 
	}
	else
		printf("Socket successfully binded..\n");
	if ((listen(sockfd, 5)) != 0) 
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli); 
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) 
	{
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");
	bzero(buff, MAX); 
	read(connfd, buff, sizeof(buff));
	int i=0,newl=0;
	printf("From client: %sTo client : ", buff);
	while(buff[i]!='.')
	{
		if(buff[i]=='\n' || buff[i]==' |')
			newl++;
		if(isalpha(buff[i]))
		{
			alp++;
			if(tolower(buff[i])=='a' || tolower(buff[i])=='e' || tolower(buff[i])=='i' || tolower(buff[i])=='o' || tolower(buff[i])=='u')
				vow++;			
		}
		else if(isdigit(buff[i]))
			num++;
		else
			other++;
		i++;	
	}
	other=other-newl;
	bzero(buff, sizeof(buff));
	//buff="No of alphabets: %d\nNumber of vowels: %d\nNumber of digits: %d\nNumber of other characters: %d\n",
	//printf("Enter the string : ");
	snprintf(buff,sizeof(buff),"No of alphabets: %d\nNumber of vowels: %d\nNumber of digits: %d\nNumber of other characters: %d",alp,vow,num,other);
	//n = 0;
	//while ((buff[n++] = getchar()) != '.');
	write(connfd, buff, sizeof(buff));
	close(sockfd);
} 
