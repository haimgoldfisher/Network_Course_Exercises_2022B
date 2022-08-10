#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 // num of writings
#define PORT 9999

int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  size_t len; 
  int count;
  
  sock = socket(AF_INET, SOCK_STREAM, 0); // TCP socket opening
    
  bzero(&serv_name, sizeof(serv_name)); // reset values to avoid strange values
  serv_name.sin_family = AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used                                  
  serv_name.sin_port = htons(PORT); // set port using the given port

  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)); // binding (for avaible port)
          
  listen(sock, 1); // listening to one socket

  len = sizeof(serv_name);
  
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len); // accept the connection

  for (count = 1; count <= SIM_LENGTH; count++)
    { write(connect_sock, &count, 4); // WRITE the count value INTO the socket, so the client will be able to read it
      printf("Server has written %d to socket.\n", count); // print what the SERVER wrote into the socket
    }

  close(connect_sock); 
  close(sock);

}
