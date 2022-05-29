#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <time.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 

#define SERVER_PORT 5060  //The port that the server listens
#define size 1024


 int main(){

      // Open the listening (server) socket
    int listeningSocket = -1;  
	 
    if((listeningSocket = socket(AF_INET , SOCK_STREAM , 0 )) == -1)
    {
        printf("Could not create socket" );
        exit(1);
    }

    // Reuse the address if the server socket on was closed
	// and remains for 45 seconds in TIME-WAIT state till the final removal.
	//
    int enableReuse = 1;
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) < 0)
    {
         printf("setsockopt() failed");
         exit(1);
    }

      // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);  //network order
      
    // Bind the socket to the port with any IP at this port
    if (bind(listeningSocket, (struct sockaddr *)&serverAddress , sizeof(serverAddress)) == -1)
    {
        printf("Bind failed");
        exit(1);
    }

    printf("Bind() success\n");

     // Make the socket listening; actually mother of all client sockets.
    if (listen(listeningSocket, 500) == -1) //500 is a Maximum size of queue connection requests
											//number of concurrent connections 
    {
    	printf("listen() failed ");
        exit(1);

    
    }

    printf("Waiting for incoming TCP-connections...\n");
      
    struct sockaddr_in clientAddress;  //
    socklen_t clientAddressLen = sizeof(clientAddress);
int flag = 0;
for(int t=0;t<2;t++){ 
   double avg=0;
char buf[256];
clock_t begin, end;


  for(int k=0;k<5;k++)
  {
   
    double cpu_time_used = 0;
    memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);

        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    	if (clientSocket == -1)
    	{
           printf("accept failed");
           exit(1);
        }

//char buf[256];
    socklen_t len;
    len = sizeof(buf); 
    if (getsockopt(clientSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    exit(1);
    } 

    printf("Current: %s\n", buf); 
    
    if(flag == 1){
    strcpy(buf, "reno");       
    }
    len = strlen(buf);
    if (setsockopt(clientSocket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
     perror("setsockopt"); 
     exit(1);
}
    len = sizeof(buf); 
    if (getsockopt(clientSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
    perror("getsockopt"); 
    exit(1); 
}
printf("New: %s\n", buf); 
    
    char buff[size];
    //recive the file in bytes
    int fp=-1;
    begin=0;
    end = 0;
    begin = clock();
    while(fp!=0){
    fp =  recv(clientSocket, buff, size, 0);
    }
    end = clock();
    cpu_time_used = ((double) (end - begin)) / CLOCKS_PER_SEC; 
    avg+=cpu_time_used;
    printf("the time of %s is %f\n",buf,cpu_time_used);

}
printf("avrage time for %s is %f \n",buf,avg/5);

flag = 1;
}
close(listeningSocket);
     return 0;
 }