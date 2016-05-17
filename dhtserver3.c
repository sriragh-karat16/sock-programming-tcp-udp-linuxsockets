#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<netdb.h>

#define BUFFERSIZE 4096
#define QUEUE_LIMIT 10
#define PORT1 23009


struct token {
	char key[20];
	char value[20];
}t[5];
int main()
{//reused code from :http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.htmlS

int j=0;
int k;
        char fname[]={"server3.txt"};
        FILE *fs;
	int find_result = 0;
	char store1[BUFFERSIZE]="";
	char store2[BUFFERSIZE]="";
	int l=0;
	int i=0;		
if((fs = fopen(fname, "r")) == NULL)
 {
		return(-1);
}           
    while(l<4) {
	fgets(store1,BUFFERSIZE,fs);
	char *key1=strtok(store1," ");
	strcpy(t[l].key, key1);
	char *value1=strtok(NULL," ");
	strcpy(t[l].value, value1);
	l++;


}

if(fs) {
		fclose(fs);
	}


int socktcp2, newsocktcp2,n;
socklen_t len;
char buffer[BUFFERSIZE]="";
char buffer4[BUFFERSIZE]="";
struct sockaddr_in serveraddr,serveraddr3;
struct sockaddr_in clientaddr;
struct hostent *server;
char ip[INET_ADDRSTRLEN];
char post[20]="POST ";
char get[20]="";
int s1;
//Create TCP connection

if((socktcp2 = socket(AF_INET, SOCK_STREAM, 0))<0)
{
perror("ERROR opening socket");
}
memset(buffer,0,BUFFERSIZE);
memset((char *)&serveraddr,0, sizeof(serveraddr));


//Server 3 details
serveraddr.sin_family = AF_INET;
serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
serveraddr.sin_port = htons(PORT1);

//obtain IP address
server=gethostbyname("nunki.usc.edu");
memcpy((void *)&serveraddr3.sin_addr,server->h_addr_list[0],server->h_length);


//Print the IP address and port number of the server
inet_ntop(AF_INET,&(serveraddr3.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 3 has TCP port number 23009 and IP address %s\n",ip);

//Binding
if (bind(socktcp2, (struct sockaddr *) &serveraddr,sizeof(serveraddr)) < 0)
{
perror("ERROR on binding");
}



while(1)
{
//Listen to the incoming connections
listen(socktcp2,QUEUE_LIMIT);

len = sizeof(clientaddr); 

//Accept the incoming connections
newsocktcp2 = accept(socktcp2,(struct sockaddr *) &clientaddr,&len);

if (newsocktcp2 < 0)
{
perror("ERROR on accept");
}

memset(buffer,0,BUFFERSIZE);
memset(buffer4,0,BUFFERSIZE);

//Receive the buffer from Server 2
n = recv(newsocktcp2,buffer,BUFFERSIZE-1,0);
sscanf(buffer,"%s %s",get,buffer4);
printf("%s\n",buffer4);
if (n < 0)
{
perror("ERROR reading from socket");
}

//print the IP and port number of the client
inet_ntop(AF_INET,&(serveraddr3.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 3 has received a request with %s from Server 2 with port number %d and IP address %s\n",buffer,ntohs(clientaddr.sin_port),ip); 

//Search the table for the required key value
j=0;
for(i=0;i<4;i++)
{
k=strncmp(buffer4,t[i].key,5);
if(k==0)
strcat(post,t[i].value);
else 
j++;
}

//Print when Server 3 has the entries in its database
if(j<4)
{
post[strlen(post)]=0;
printf("\nThe Server 3 sends the reply %s to Server 2 with port number %d and IP address %s\n",post,ntohs(clientaddr.sin_port),ip);
sendto(newsocktcp2,post,strlen(post),0,(struct sockaddr *)&clientaddr,len);

//Initialise all variables after operation
strcpy(post,"POST ");
strcpy(buffer,"");
strcpy(buffer4,"");
strcpy(get,"");
}
else
printf("\nThe value for the key is not available\n");
}
//Close the child socket
close(newsocktcp2);
//Close the parent socket
close(socktcp2);

}
