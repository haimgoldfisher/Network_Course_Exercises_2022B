
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res; // ptr to the 'getaddrinfo' function's result
  char* hostname; // ptr to the host anme
  char* hostaddr; // ptr to the address of the host name
  struct sockaddr_in* saddr; // socket address ptr for the address
  
  if (argc != 2) { // err catching - for wrong cmd command case
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1]; // from cmd 
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { // err catching - for wrong hostname case 
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
  // the given output of the 'getaddrinfo' func: 
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Address for %s is %s\n", hostname, hostaddr); // hostname + ip address
  exit(0);
}
