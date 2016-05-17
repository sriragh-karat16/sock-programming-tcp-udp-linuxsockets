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
#define QUEUE_LIMIT 1
#define PORT1 22009
#define PORT2 23009

struct token {
	char key[20];
	char value[20];
}t[50];
int main()
{//reused code from :http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.htmlS

int socktcp2,newsocktcp2,newsocktcp,n;
socklen_t len2,len3;
char buffer[BUFFERSIZE]=""; 
char buffer2[BUFFERSIZE]=""; 
char buffer3[BUFFERSIZE]=""; 
char buffer4[BUFFERSIZE]="";
char buffer5[BUFFERSIZE]="";  
struct sockaddr_in serveraddr;
struct sockaddr_in clientaddr2;
struct sockaddr_in serveraddr1,serveraddr2,serveraddr3;
struct hostent *server;
int result;
int j=0;
char fname[]={"server2.txt"};
FILE *fs;
char store1[BUFFERSIZE]="";
char store2[BUFFERSIZE]="";
char ip[INET_ADDRSTRLEN];
int l=0;
int i=0;	
int k;
int z=4,s1;	
char post[20]="POST  ";
char get[20]="";
char get2[20]="GET ";
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

//Create Server 2 socket
socktcp2 = socket(AF_INET, SOCK_STREAM, 0);

if(socktcp2<0)
{
perror("ERROR opening socket");
}
memset(buffer,0,BUFFERSIZE);
memset((char *)&serveraddr,0, sizeof(serveraddr));

//Server 2 details
serveraddr.sin_family = AF_INET;
serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
serveraddr.sin_port = htons(PORT1);

//obtain IP address
server=gethostbyname("nunki.usc.edu");
memcpy((void *)&serveraddr3.sin_addr,server->h_addr_list[0],server->h_length);


//Print the IP address and port number of the server
inet_ntop(AF_INET,&(serveraddr3.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 2 has TCP port number 22009 and IP address %s\n",ip);

//Binding the server
if(bind(socktcp2, (struct sockaddr *) &serveraddr,sizeof(serveraddr))<0) 
{
perror("ERROR on binding");
}

while(1)
{
//Listening 
listen(socktcp2,QUEUE_LIMIT);

len2 = sizeof(clientaddr2); 

//Accept the incoming connections
newsocktcp2 = accept(socktcp2,(struct sockaddr *)&clientaddr2,&len2);
if (newsocktcp2 < 0)
{
perror("ERROR on accept");
} 
memset(buffer,0,BUFFERSIZE);
memset(buffer4,0,BUFFERSIZE);

//Receive values from server 1
n = recv(newsocktcp2,buffer,BUFFERSIZE-1,0);

//Split "get" from the key 
sscanf(buffer,"%s" "%s",get,buffer4);

if (n < 0)
{
perror("ERROR reading from socket");
}

//print the IP and port number of Server 1
inet_ntop(AF_INET,&(serveraddr3.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 2 has received a request with %s from Server 1 with port number %d and IP address %s\n",buffer4,ntohs(clientaddr2.sin_port),ip); 


//Search the local database whether the key-value is available
j=0;
for(i=0;i<z;i++)
{
k=strncmp(buffer4,t[i].key,5);
if(k==0)
strcat(post,t[i].value);
else
j++;
}

//Send the data to Server 1
if(j<z)
{
post[strlen(post)]=0;
inet_ntop(AF_INET, &(clientaddr2.sin_addr), ip, INET_ADDRSTRLEN);
printf("\nThe Server 2 sends the reply %s to Server 1 with port number %d and IP address %s\n",post,ntohs(clientaddr2.sin_port),ip);
sendto(newsocktcp2,post,strlen(post),0,(struct sockaddr *)&clientaddr2,len2);

//Initialise all variables after operation
strcpy(post,"POST  ");
strcpy(buffer,"");
strcpy(get,"");
strcpy(get2,"GET ");
}
if(j>=z)
{
//TCP client
//Create TCP client socket
newsocktcp=socket(AF_INET,SOCK_STREAM,0);

//Finds IP address
server = gethostbyname("nunki.usc.edu");
memcpy((void *)&serveraddr2.sin_addr,server->h_addr_list[0],server->h_length);


memset((char *) &serveraddr1,0,sizeof(serveraddr1));

//Server 3 details
serveraddr1.sin_family = AF_INET;
memcpy((char *)server->h_addr,(char *)&serveraddr1.sin_addr.s_addr,server->h_length);
serveraddr1.sin_port = htons(PORT2);

//Connect with server 3
if (connect(newsocktcp,(struct sockaddr *)&serveraddr1,sizeof(serveraddr1)) < 0)
{
printf("ERROR connecting");
}

//Send the data to the server 3

strcat(get2,buffer4);
n = send(newsocktcp,get2,strlen(get2),0);

if (n < 0)
{
printf("ERROR writing to socket");
}
len3=sizeof(serveraddr2);
printf("\nThe Server 2 sends the request %s to the Server 3\n",get2); 


memset(buffer,0,BUFFERSIZE);

//Receive value from the server 3
n = recv(newsocktcp,buffer,BUFFERSIZE-1,0);
if (n < 0)
{
printf("ERROR reading from socket");
}

//Obtain port number and IP
getsockname(newsocktcp, (struct sockaddr *)&serveraddr2,&len3);
inet_ntop(AF_INET, &(serveraddr2.sin_addr), ip, INET_ADDRSTRLEN);

printf("\nThe TCP port number is %d and the IP address is %s\n",ntohs(serveraddr2.sin_port),ip);


//Close the client socket
close(newsocktcp);

//print data received from server 3
sscanf(buffer,"%s %s",post,buffer5);
buffer[strlen(buffer)]=0;
inet_ntop(AF_INET, &(serveraddr3.sin_addr), ip, INET_ADDRSTRLEN);
printf("\nThe Server 2 recieved the %s from the Server 3 with port number 23009 and IP address %s\n",buffer,ip); 
printf("\nThe Server 2 closed the TCP connection with Server 3\n");



//Send the received value to server 1
n=send(newsocktcp2,buffer,strlen(buffer),0);
inet_ntop(AF_INET, &(serveraddr3.sin_addr), ip, INET_ADDRSTRLEN);
printf("\nThe Server 2 sent reply %s to Server 1 with port number %d and IP address %s\n",buffer,ntohs(clientaddr2.sin_port),ip); 

//Updating the cache
if(j>3)
{
strcpy(t[z].key,buffer4);
strcpy(t[z].value,buffer5);
z++;
}
//Initialise all variables after operation
strcpy(post,"POST  ");
strcpy(buffer,"");
strcpy(buffer5,"");
strcpy(buffer4,"");
strcpy(get,"");
strcpy(get2,"GET ");

}

//Close the child socket
close(newsocktcp2);
}

//Close parent socket
close(socktcp2);
}


