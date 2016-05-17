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
#define PORT1 21009
char buffer[BUFFERSIZE]="";
char buffer2[BUFFERSIZE]="";

//Search program to find the corresponding key of the text entered 
int search() 
{
        char fname[]={"client1.txt"};
        FILE *fp;
	char temp[BUFFERSIZE];
	int j;
	int i=BUFFERSIZE;
	if((fp = fopen(fname, "r")) == NULL)
 {
		return(-1);
}
//Reads the file and finds key corresponding to the text		
                while(fgets(temp,BUFFERSIZE,fp)!=NULL) {
		if((strstr(temp,buffer)) != NULL) {
		        for(j=0;j<10;j++)
			buffer[j]=temp[j+5];
			}			
	}
		

	
	if(fp) {
		fclose(fp);
	}
return 1;
}


//main program
int main()
{//reused code from :http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.htmlS

int sockudpc,n,portno;
socklen_t length;
struct sockaddr_in serverudp,sender,clientaddr2,clientaddr3,sender2;
struct hostent *hp;
int i;
int result;
char get[20]="GET ";
char post[20]="";
char ip1[INET_ADDRSTRLEN];
int s1;
int flen2;
//Creates the client UDP socket
sockudpc= socket(AF_INET, SOCK_DGRAM, 0);
if(sockudpc<0)
{
printf("Error in socket descriptor\n");
fflush(stdout);
}



if(hp==0)
{
printf("unknown host");
fflush(stdout);
}

//Client details
clientaddr2.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
clientaddr2.sin_port=htons(0);
clientaddr2.sin_family=AF_INET;

//finds the IP address
hp=gethostbyname("nunki.usc.edu");
memcpy((void *)&clientaddr2.sin_addr,hp->h_addr_list[0],hp->h_length);




bind(sockudpc,(struct sockaddr *)&clientaddr2,sizeof(clientaddr2));



//server details
serverudp.sin_family=AF_INET;
memcpy((char *)hp->h_addr,(char *)&serverudp.sin_addr,hp->h_length);
serverudp.sin_port = htons(PORT1);
length=sizeof(struct sockaddr_in);

memset(buffer,0,BUFFERSIZE);
memset(buffer2,0,BUFFERSIZE);



//Search word is entered here
printf("\nPlease enter your search:\n");
scanf("%s",buffer);
strcpy(buffer2,buffer);


//Transfers the result to the search program where it searches for the keyword
result=search();

if(result==0)
{
printf("Key unavailable\n");
fflush(stdout);
exit(0);
}

printf("\nThe Client has received a request with search word %s ,which maps the corresponding %s\n",buffer2,buffer);
strcat(get,buffer);

//Sending data to Server 1
n=sendto(sockudpc,get,strlen(get),0,(struct sockaddr *)&serverudp,length);


if(n<0)
{
printf("Error in send to");
fflush(stdout);
exit(1);
}
//Receive the reply from Server 1
n=recvfrom(sockudpc,post,BUFFERSIZE-1,0,(struct sockaddr *)&sender, &length);


flen2=sizeof(clientaddr2);
getsockname(sockudpc, (struct sockaddr *)&clientaddr2, (socklen_t *)&flen2);
inet_ntop(AF_INET, &(clientaddr2.sin_addr), ip1, INET_ADDRSTRLEN);

printf("\nThe Client 2 sends the request %s to the Server 1 with port number 21009 and IP address %s\n",get,ip1);
printf("\nThe Client 2's port number is %d and the IP address is %s\n",ntohs(clientaddr2.sin_port),ip1);


inet_ntop(AF_INET,&(clientaddr2.sin_addr),ip1,INET_ADDRSTRLEN);
printf("\n The Client 2 received the value %s from the Server 1 with port number 21009 and IP address %s\n",post,ip1);
printf("\nThe Client 2's port number is %d and the IP address is %s\n",ntohs(clientaddr2.sin_port),ip1);

close(sockudpc);
return 0;
}
