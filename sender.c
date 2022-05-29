#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "127.0.0.1"

#define size 1024


int main(){
int flag = 0;    

for(int k=0;k<2;k++){
    
    for(int t=0;t<5;t++){
     //create socket:
	//AF_INTY = struct of ipv4
	//SOCK_STREAM = struct of tcp 
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //if the function "socket" return -1 that means the socket was not created.
     if(sock == -1)
    {
    printf("socket was not created");
    exit(1);
    }
   //change the Congestion Control
   ///////////////////////////////////////////////////////////////s
    
    char buf[256];
    socklen_t len;


    len = sizeof(buf); 
    if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    exit(1);
    } 

    printf("Current: %s\n", buf); 
    
    if(flag == 1){
    strcpy(buf, "reno");       
    }
    len = strlen(buf);
    if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
     perror("setsockopt"); 
     exit(1);
}
    len = sizeof(buf); 
    if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
    perror("getsockopt"); 
    exit(1); 
}
printf("New: %s\n", buf); 

////////////////////////////////////////////////////////////////////////
    //Structures for handling internet addresses
	//ipv4, port and internet address.    
    struct sockaddr_in serverAddress;

    //memset is to reset the address
    memset(&serverAddress, 0, sizeof(serverAddress));

    //sin_family=AF_INET ,ipv4 and dest port
    serverAddress.sin_family = AF_INET;

    //sin_port = the server port in bytes.
    serverAddress.sin_port = htons(SERVER_PORT);

    //a function to convert the internet address from its standard text format to its numeric binary form
	int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
	if (rval <= 0)
	{
		printf("inet_pton() failed");
		exit(1);
	}
    //another option is inet

    // Make a connection to the server with the socket.
     if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
     {
	   printf("connect() failed ");
       exit(1);
     }
     printf("connected to server\n");


    FILE *fp;
    //open the file
    fp = fopen("file.txt","r");
    if(fp==NULL){
        printf("couldnt open the file");
        exit(1);
    }

   // int leng=0;
    int read;
    char buff2[size];
    int bytesSent=0;

    while((read=fread(buff2, 1,sizeof(buff2),fp))>0){
    //sending the file in bytes    
    bytesSent += send(sock,buff2,read, 0);
    //counting the bytes
    //if  send() is negetive there is a problem with the function
    if(bytesSent<0){
        printf("there is a problem with sending the file");
        exit(1);
    }
    }

    //print how much bytes were sent
    
        printf("%d bytes was sent",bytesSent);
        
   
    //close the socket
    close(sock);
    
}
flag = 1;

}
    return 0;
}