#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<netdb.h>
#define PORT1 21009
#define PORT2 22009
#define BUFFERSIZE 4096

 struct token {
	char key[20];
	char value[20];
}t[50];

int main()
{//Reused code from :http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
int j=0;
int i=0;
int z=4;
int k;
char store1[BUFFERSIZE]="";
int l=0;
char buffer2[BUFFERSIZE]="";
char buffer3[BUFFERSIZE]="";
char buffer4[BUFFERSIZE]="";
char buffer5[BUFFERSIZE]="";
char fname[]={"server1.txt"};
FILE *fs;
int find_result = 0;
int sockudp,newsocktcp2,n;
socklen_t flen,flen2; 
struct sockaddr_in serverudp,serveraddr1,serveraddr2,serveraddr3;
struct sockaddr_in sender,sender2;
struct hostent *server,*hp;
char buffer[BUFFERSIZE]="";
char store2[BUFFERSIZE]="";
char ip[INET_ADDRSTRLEN];
char post[20]="POST ";
char get[20]="";
char get2[20]="GET ";
int result;
int length,cli;
int count=1;		
int s1;
if((fs = fopen(fname, "r")) == NULL)
 {
		return(-1);
}            
        strcpy(buffer2,"");
	strcpy(buffer3,"");
	
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

//create UDP server socket
sockudp=socket(AF_INET, SOCK_DGRAM, 0);
if (sockudp < 0)
{
printf("Error opening socket\n");
}

memset(buffer,0,BUFFERSIZE);
memset((char *)&serverudp,0,sizeof(serverudp));

//set up the server
serverudp.sin_family=AF_INET;
serverudp.sin_addr.s_addr=htonl(INADDR_ANY);
serverudp.sin_port=htons(PORT1);

//obtain IP address
server=gethostbyname("nunki.usc.edu");
memcpy((void *)&serveraddr3.sin_addr,server->h_addr_list[0],server->h_length);


//Print the IP address and port number of the server
inet_ntop(AF_INET,&(serveraddr3.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 1 has UDP port number 21009 and IP address %s\n",ip);

//bind the UDP socket
if (bind(sockudp,(struct sockaddr *)&serverudp,sizeof(serverudp))<0)
{
printf("Binding error\n");
}


flen = sizeof(sender);
while(1)
{
memset(buffer,0,BUFFERSIZE);
memset(buffer4,0,BUFFERSIZE);

//receive data from the socket
n=recvfrom(sockudp,buffer,BUFFERSIZE-1,0,(struct sockaddr *)&sender,&flen);
if (n < 0) 
{ 
printf("Error in recieve from\n"); 
}
//split "get" and the key in the buffer
sscanf(buffer,"%s %s",get,buffer4);

//To differentiate between client 1 and 2
if((count%2)==0)
cli=2;
else 
cli=1;

//print the IP and port number of the client
inet_ntop(AF_INET,&(sender.sin_addr),ip,INET_ADDRSTRLEN);
printf("\nThe Server 1 has received a request with %s from client %d with port number %d and IP address %s\n",buffer4,cli,ntohs(sender.sin_port),ip); 

//print if the server has entry in its table
j=0;
for(i=0;i<z;i++)
{
k=strncmp(buffer4,t[i].key,5);
if(k==0)
strcat(post,t[i].value);
else
j++;
}

//Send reply to client
if(j<z)
{

post[strlen(post)]=0;
printf("\nThe Server 1 sends the reply %s to Client %d with port number %d and IP address %s\n",post,cli,ntohs(sender.sin_port),ip);
sendto(sockudp,post,strlen(post),0,(struct sockaddr *)&sender,flen);

//Initialise all variables after operation
strcpy(post,"POST ");
strcpy(buffer,"");
strcpy(get,"");
strcpy(get2,"GET ");
count+=1;
}
if(j>=z) 
{
//TCP client

//create TCP socket
newsocktcp2=socket(AF_INET,SOCK_STREAM,0);

//obtain IP address
memset((char *)&serveraddr2,0,sizeof(serveraddr2));
hp=gethostbyname("nunki.usc.edu");
memcpy((void *)&serveraddr2.sin_addr,hp->h_addr_list[0],hp->h_length);


memset((char *)&serveraddr1,0,sizeof(serveraddr1));


//Server2 details
serveraddr1.sin_family=AF_INET;
memcpy((char *)server->h_addr,(char *)&serveraddr1.sin_addr.s_addr,server->h_length);
serveraddr1.sin_port = htons(PORT2);


//Connect to Server 2
if (connect(newsocktcp2,(struct sockaddr *)&serveraddr1,sizeof(serveraddr1)) < 0)
{
printf("ERROR connecting\n");
}
flen2=sizeof(serveraddr1);


strcat(get2,buffer4);
//Send data to server 2
get2[strlen(get2)]=0;
n = sendto(newsocktcp2,get2,strlen(get2),0,(struct sockaddr *)&serveraddr1,flen2);
if (n < 0)
{
printf("ERROR writing to socket\n");
}
printf("\nThe Server 1 sends the request %s to the Server 2\n",buffer); 



memset(buffer,0,BUFFERSIZE);


//Recieve data from server 2
n = recvfrom(newsocktcp2,buffer,BUFFERSIZE-1,0,(struct sockaddr *)&serveraddr1,&flen2);
if (n < 0)
{
printf("ERROR reading from socket\n");
}

//Obtain Server 1 dynamic port number
getsockname(newsocktcp2, (struct sockaddr *)&serveraddr2, &flen2);
inet_ntop(AF_INET, &(serveraddr2.sin_addr), ip, INET_ADDRSTRLEN);
printf("\nThe TCP port number is %d and the IP address is %s\n",ntohs(serveraddr2.sin_port),ip);

//closing the TCP socket
close(newsocktcp2);



memset(buffer5,0,BUFFERSIZE);


//print data received from server2
sscanf(buffer,"%s %s",post,buffer5);
buffer5[strlen(buffer5)]=0;
inet_ntop(AF_INET, &(serveraddr3.sin_addr), ip, INET_ADDRSTRLEN);
printf("\nThe Server 1 recieved the %s from the Server 2 with port number 22009 and IP address %s\n",buffer,ip); 
printf("\nThe Server 1 closed the TCP connection with the Server 2\n");


//Storing in the database
if(j>3)
{

strcpy(t[z].key,buffer4);
strcpy(t[z].value,buffer5);
z++;
}


//Sending to the client 
sendto(sockudp,buffer,strlen(buffer),0,(struct sockaddr *)&sender,flen);
inet_ntop(AF_INET, &(serveraddr2.sin_addr), ip, INET_ADDRSTRLEN);
buffer[strlen(buffer)]=0;
printf("\nThe Server 1 sent reply %s to client %d with port number %d and IP address %s\n",buffer5,cli,ntohs(sender.sin_port),ip); 


//Initialise all variables after operation
strcpy(post,"POST ");
strcpy(buffer,"");
strcpy(buffer5,"");
strcpy(buffer4,"");
strcpy(get,"");
strcpy(get2,"GET ");
count+=1;
}
}
}
