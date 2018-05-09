/* 
StudentNumber:515020910156
Name:庄丁奕
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <ctype.h>
#include <string.h>
/*======Question====*/
/*
Please develop a Caesar Encryption Server, which receives plaintext from clients and
sends the corresponding ciphertext to clients.
Only the letters need to be encrypted, e.g. How are you? → Krz duh brx?
The Server can serve at most 2 clients concurrently, more clients coming have to wait.
The server-side program must be concurrent multi-threaded.
*/
pthread_mutex_t serveMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t threadStatusMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t threadStatusWritingMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t clientThreads[2];// Holds 2 clients
void *nothing; // parameter for pthread_exit

void *encryption(char buffer[],char buffer_ecp[]);
void *serve(void * newsockfd);
int checkStatus();
void *setStatus(int status,void *sockfd);
void *exitStatus();
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
// used this struct to convey multi-parameters through pthread_create()
struct statusPara{
	int sockfd;
	int status; 
};
//Check whether there is a vacant position for the coming thread
int checkStatus(){
pthread_mutex_lock(&threadStatusMutex);
	if (clientThreads[0]==0){
		pthread_mutex_unlock(&threadStatusMutex);
		return 0;
	}
	else if (clientThreads[1]==0){
		pthread_mutex_unlock(&threadStatusMutex);
		return 1;
	}
	else{
		pthread_mutex_unlock(&threadStatusMutex);
		return -1;
	}
}

void *setStatus(int status,void *sockfd){
	int newsockfd=(int)(*((int*)sockfd));
pthread_mutex_lock(&threadStatusWritingMutex);
	if (status>=0 && status <=1)
		clientThreads[status]=1;
	else{
		char *reply="Busy now!Please retry a few moments later...\n";// turn off the connection if it is full
		int n;
		n = write(newsockfd,reply,strlen(reply));
		if (n < 0) error("ERROR writing to socket");
		close(newsockfd);// If a thread cannot be connected, kill it.
	}
pthread_mutex_unlock(&threadStatusWritingMutex);
}

void *exitStatus(int status){ // If a thread leaves, update the thread status into 0, we do not need to specify the particular order of threads to quit
pthread_mutex_lock(&threadStatusWritingMutex);
	clientThreads[status]=0;
pthread_mutex_unlock(&threadStatusWritingMutex);
}

int main(int argc, char *argv[]){
	int sockfd,newsockfd,portno,clilen,n,status; //status used for checking the threads status
	struct sockaddr_in serv_addr,cli_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0){
		printf("ERROR opening socket\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	portno = 2050;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	if (bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
		printf("Error on binding\n");
		exit(1);
	}
	listen(sockfd,2); // At most 2 clients waiting. 
	clilen=sizeof(cli_addr);
	printf("Server initiating...\n");
	// Begin multi-thread service here
	for (int i=0;i<2;++i){
		clientThreads[i]=0; // Initialize the client threads status, 0 means empty, 1 means full
	}
	while (1)
	 {
	   newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen); // here is a queue for request
	   if (newsockfd < 0)
		error("ERROR on accept");
	   status=checkStatus();// Check whether there is empty threads to hold
	   setStatus(status,&newsockfd);// Hereafter, thread status is defined
	   // Begin creating threads and their function serve
	   if (status>=0 && status <=1){
		int  iret;
		struct statusPara pstru;
		pstru.sockfd=newsockfd;
		pstru.status=status;
     		if((iret= pthread_create(&clientThreads[status], NULL, serve,&pstru))){
        		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
         		exit(EXIT_FAILURE);
   		}
	   }
	   
	 }


	close(sockfd);
	return 0;

}
void *encryption(char buffer[],char buffer_ecp[]){
		int i=0;
		int j;
		for (i=0;i<strlen(buffer)-1;++i){
			j=buffer[i];
			if (j>64&&j<88 || j>96&&j<120)
				buffer_ecp[i]=j+3;
			else if (j>87&&j<91 || j>119&&j<123)
				buffer_ecp[i]=j-23;
			else
				buffer_ecp[i]=buffer[i];
		
		}
}

void *serve(void *arg){
	struct statusPara *ps;
	ps=(struct statusPara *)arg;
	
	int newsockfd=(*ps).sockfd;
	int newstatus=(*ps).status;
	//int newsockfd=(int)(*((int*)(sockfd)));
	int n;
	char buffer[256];
	char buffer_ecp[256];
	// Services provided for each client
	while(1){
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) {
			error("ERROR reading from socket");
			break;
		}	
		// exit
		if (n==0) {// if cliet types ":q" nothing conveyed, which results in n=0
				printf("Sever thread closing...\n");
				exitStatus(newstatus); // release the threads to other clients
				close(newsockfd);
				pthread_exit(nothing);
				break;			
			}
		else{
		printf("Receiving message: %s",buffer);
		// Encryption
		encryption(buffer,buffer_ecp);
		char *temp="From server: ";
		char *str;
		str=(char *) malloc(strlen(temp)+strlen(buffer_ecp)); // neglecting LF
		strcpy(str,temp);
		strcat(str,buffer_ecp);
		 	n = write(newsockfd,str,strlen(str));
		 	if (n < 0) error("ERROR writing to socket");
		}
	}
}
