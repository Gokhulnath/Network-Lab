// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <math.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT	 8080 
#define MAXLINE 1024 
#define MAXBIT 32

int hamming_code(int msg[],int *result, int n){
	int r=0,i=0;
	while(n>(int)pow(2,i)-(i+1))
	{
		r++;
		i++;
	}
	n=n+r;
	int temp=0;
	for(int i=0;i<n;i++){
		int a=n-i;
		if(a && (!(a&(a-1)))){
				result[i]=-1;
		}
		else{
			result[i]=msg[temp];
			temp++;
		}
	}
	
	for(int i=0;i<r;i++){
		int ones=0;
		for(int j=n-(int)pow(2,i);j>=0;j-=((int)pow(2,i))*2){
			for(int k=j;k>j-(int)pow(2,i);k--){
				if(result[k]==1){
					ones++;
				}
			}
		}
		if(ones%2==0){
			result[n-((int)pow(2,i))]=0;
		}else{
			result[n-((int)pow(2,i))]=1;
		}
	}
	return r;
}

int error_detection(int *result,int n,int r){
	int error_code[r];
	int temp=r-1;
	n=n+r;
	for(int i=0;i<r;i++){
		int ones=0;
		for(int j=n-(int)pow(2,i);j>=0;j-=((int)pow(2,i))*2){
			for(int k=j;k>j-(int)pow(2,i);k--){
				if(result[k]==1){
					ones++;
				}
			}
		}
		if(ones%2==0){
			error_code[temp]=0;
		}else{
			error_code[temp]=1;
		}
		temp--;
	}
	int error_bit=0;
	for(int i=0;i<r;i++){
		error_bit+=(int)pow(2,r-i-1)*error_code[i];
	}
	if(result[n-error_bit]==0){
		result[n-error_bit]=1;
	}
	else{
		if(result[n-error_bit]==1){
			result[n-error_bit]=0;
		}
	}
	return error_bit;
}




// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 
	//hamming code variables
	int n, r, i,e;
    int msg[MAXBIT];
	int result[MAXBIT];
	int error_code[MAXBIT];
	int error[MAXBIT];

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
	
	int len; 
	while(1){
		bzero(msg, sizeof(msg));
		printf("\n---------------------------------------------\n");
		printf("\nEnter ur code to server\n");
		
		printf("Enter number bits for message:");
		scanf("%d", &n);
		printf("\nEnter the message:\n");
		for(int i = 0; i < n; i++)
		{
			scanf("%d",&msg[i]);
		}
		int choice=-1;
		while(choice!=4){
			printf("\n---------------------------------------------\n");
			printf("\n1.Generate hamming code.\n2.Check for error bit.\n3.Insert an error bit.\n4.Exit\nYour choice: ");
			scanf("%d",&choice);
			switch(choice){
				case 1:
						r=hamming_code(msg,result,n);
						printf("Length of message: %d\nLength of redundant bits: %d\nThe hamming code: ",n,r);
						for(int i = 0; i < n+r; i++)
						{
							printf("%d", result[i]);
						}
						printf("\n");
						break;
				case 2:
						
						for(int i=0;i<n+r;i++){
							error[i]=result[i];
						}
						e=error_detection(result,n,r);
						if(e==0){
							printf("\nNo error detected!\n");
						}
						else{
							printf("\nError at %d bit\n",e);
							printf("\nHamming code with error: ");
							for(int i = 0; i < n+r; i++)
							{
								printf("%d", error[i]);
							}
							printf("\n");
							printf("\nCorrected hamming code: ");
							for(int i = 0; i < n+r; i++)
							{
								printf("%d", result[i]);
							}
							printf("\n");
						}
						break;
				case 3:
						printf("\nYou can insert error bit between 1 to %d bits\n",n+r);
						printf("\nEnter the  bit to be changed as error: ");
						int user_error;
						scanf("%d",&user_error);
						for(int i=0;i<n+r;i++){
							error_code[i]=result[i];
						}
						if(error_code[n+r-user_error]==0){
							error_code[n+r-user_error]=1;
						}
						else{
							if(error_code[n+r-user_error]==1){
								error_code[n+r-user_error]=0;
							}
						}
						printf("\nCode before adding error: ");
						for(int i = 0; i < n+r; i++)
						{
							printf("%d", result[i]);
						}
						printf("\n");
						printf("\nErrored at %d bit code: ",user_error);
						for(int i = 0; i < n+r; i++)
						{
							printf("%d", error_code[i]);
						}
						printf("\n");
						for(int i=0;i<n+r;i++){
							result[i]=error_code[i];
						}
						break;
				case 4:
						break;
				default:
						printf("Enter valid option");
			}
		}		
		sendto(sockfd, result, (n+r)*4, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
		printf("\nCODE SENT TO SERVER\n");
		printf("\n---------------------------------------------\n");
		printf("\nCODE RECIEVED FROM SERVER\n");
		bzero(msg, sizeof(msg));
		bzero(result, sizeof(result));
		bzero(error_code, sizeof(error_code));
		bzero(error, sizeof(error));
		n = recvfrom(sockfd, msg, sizeof(msg), MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
		n=n/4;
		printf("\nServer hamming code : ");
		for(int i = 0; i < n; i++)
		{
			printf("%d", msg[i]);
		}
		printf("\n");
		int m=0,r=0;
		while((int)pow(2,n-m)>=n+1){
			m++;
		}
		r=n-m;
		//error detection
		for(int i=0;i<m+r;i++){
			error[i]=msg[i];
		}
		e=error_detection(msg,m,r);
		if(e==0){
			printf("\nNo error detected!\n");
		}
		else{
			printf("\nError at %d bit\n",e);
			printf("\nHamming code with error: ");
			for(int i = 0; i < m+r; i++)
			{
				printf("%d", error[i]);
			}
			printf("\n");
			printf("\nCorrected hamming code: ");
			for(int i = 0; i < m+r; i++)
			{
				printf("%d", msg[i]);
			}
			printf("\n");
		}
	}
	close(sockfd); 
	return 0; 
} 

/*
output:
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop$ gcc client.c -o c -lm
gokhul@DESKTOP-C3RN1T1:/mnt/c/Users/gokhu/Desktop$ ./c

---------------------------------------------

Enter ur code to server
Enter number bits for message:8

Enter the message:
1
1
1
1
0
0
1
1

---------------------------------------------

1.Generate hamming code.
2.Check for error bit.
3.Insert an error bit.
4.Exit
Your choice: 1
Length of message: 8
Length of redundant bits: 4
The hamming code: 111100010110

---------------------------------------------

1.Generate hamming code.
2.Check for error bit.
3.Insert an error bit.
4.Exit
Your choice: 4

CODE SENT TO SERVER

---------------------------------------------

CODE RECIEVED FROM SERVER

Server hamming code : 100011111110

Error at 5 bit

Hamming code with error: 100011111110

Corrected hamming code: 100011101110

---------------------------------------------

Enter ur code to server
Enter number bits for message:7

Enter the message:
1
0
0
0
0
1
1

---------------------------------------------

1.Generate hamming code.
2.Check for error bit.
3.Insert an error bit.
4.Exit
Your choice: 1
Length of message: 7
Length of redundant bits: 4
The hamming code: 10010011101

---------------------------------------------

1.Generate hamming code.
2.Check for error bit.
3.Insert an error bit.
4.Exit
Your choice: 3

You can insert error bit between 1 to 11 bits

Enter the  bit to be changed as error: 2

Code before adding error: 10010011101

Errored at 2 bit code: 10010011111

---------------------------------------------

1.Generate hamming code.
2.Check for error bit.
3.Insert an error bit.
4.Exit
Your choice: 4

CODE SENT TO SERVER

---------------------------------------------

CODE RECIEVED FROM SERVER

Server hamming code : 1110100011010

No error detected!

---------------------------------------------

*/