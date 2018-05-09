/* 
StudentNumber:515020910156
Name:庄丁奕
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
/*======Question====*/
/*
Client send messages to sever and input :q to end the service.
*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[]){
	int sockfd,portno,n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	portno =2050;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0){
		printf("ERROR opening socket");
	exit(1);
	}
	server=gethostbyname("127.0.0.1");
	if (server == NULL){
		printf("ERROR, no such host");
	exit(0);
	}
	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port=htons(portno);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
		printf("ERROR connecting\n");
		exit(1);
	}
	printf("Please enter the message:\n");

	//Begin to send words

	while(1){
	    	bzero(buffer,256);
		fgets(buffer,255,stdin);
	
	    if (strlen(buffer)==3 && buffer[0]==':' && buffer[1]=='q'){
			printf("Client closing...\n");			
			return 0;
		
	    }
	
	    n = write(sockfd,buffer,strlen(buffer));//send to server
	    if (n < 0) 
			error("ERROR writing to socket");
	    bzero(buffer,256);
	    n = read(sockfd,buffer,255);
	    if (n < 0) 
			error("ERROR reading from socket");
	    printf("%s\n",buffer);
		bzero(buffer,256);
	}
	return 0;
}
