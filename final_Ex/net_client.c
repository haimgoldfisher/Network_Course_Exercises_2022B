
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SIM_LENGTH 10 // num of readings
// #define IP_ADDRESS "????" - WE MUST HAVE IP ADDRESS SO THE CLIENT WILL KNOW THE SERVER HOST IP
#define IP_ADDRESS "127.0.1.1"
#define PORT 9999 

int main(int argc, char* argv[]) 
{ 

  // from nslookup.c
  
  struct addrinfo* res; // ptr to the 'getaddrinfo' function's result
  char* hostname; // ptr to the host anme
  char* hostaddr; // ptr to the address of the host name
  struct sockaddr_in* saddr; // socket address ptr for the address
  
  if (argc != 2) { // err catching - WRONG CMD COMMAND CASE (must be: ./<name of the prog> + HOSTNAME)
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1]; // from cmd 
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { // err catching - WRONG HOSTNAME CASE
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
  // the given output of the 'getaddrinfo' func: 
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Address for %s is %s\n", hostname, hostaddr); // hostname + ip address

  // end of nslookup.c (without exit)
  
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 

  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0); // TCP socket opening
  if (sock < 0) // err catching - WHEN SOCKET CANNOT BE OPENED
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  bzero(&cli_name, sizeof(cli_name)); // reset values to avoid strange values
  cli_name.sin_family = AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used  
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); // WHERE TO CONNECT (SERVER IP)
  cli_name.sin_port = htons(PORT); // set port using the given port


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) // err catching - WHEN IP IS UNKNOWN WE CANNOT CONNECT
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4); // READ the count value FROM the socket
      printf("Client has received %d from socket.\n", value); // print what the CLIENT read from the socket
    }

  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
