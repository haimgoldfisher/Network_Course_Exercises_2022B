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
#define PORT 80 // = receive from a web client
#define LINE_SIZE 1024 // max length of a line

int main(int argc, char* argv[]) 
{ 
  struct addrinfo* res; // ptr to the 'getaddrinfo' function's result
  char* url; // ptr to the url
  char* hostname; // ptr to the host name
  char* hostaddr; // ptr to the address of the host name
  struct sockaddr_in* saddr; // socket address ptr for the address
  if (argc != 2) { // err catching - WRONG CMD COMMAND CASE (must be: ./<name of the prog> + URL)
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }
  url = argv[1]; // from cmd 
  char* pre = "http://"; // accept "http://" from
  char* pre2 = "https://"; // try to accept "https://" from
  char host[100]; // it holds the host name
  char path[100]; // it holds the path of the website after the host name
  int port = PORT; // default port number for web client is 80, but we can also scan a specific port name later
  if (strncmp(url, pre, strlen(pre)) == 0 || strncmp(url, pre2, strlen(pre2)) == 0) { // for "http://..." form
  /* this method ignore "http://" , divide by ':' (port case) and divide by '/' (path case)
     we accept to get 1 to 3 outputs: the hostname (always), the port (if there is a unique one, else - 80 as default) and the path (if there is one) */
    // sscanf(url, "http://%99[^:]:%99d/%99[^\n]", host, &port, path);
    sscanf(url, "%*[^:]%*[:/]%[^:]:%d%s", host, &port, path); // for http://<hostname>/<path> and http://<hostname>:<port>/<path> forms
    if (port = 80)
    {
      if (strlen(path) == 1)
      {
        sscanf(host, "%*[^/]%[^:]:", path); // take the path from host+path
        sscanf(host, "%[^/]", host); // take the host from host+path
      }
    }
    hostname = host; // update the hostname after parsing
    printf("hostname= %s, port= %d, path= %s\n", hostname, port, path);
  }
  else { // for host name form or wrong url case
    hostname = url; // update the hostname without any parsing - the url is the hostname or error case
  }

  int sock; 
  struct sockaddr_in cli_name; // for the client - the website host name
  sock = socket(AF_INET, SOCK_STREAM, 0); // TCP socket opening
  if (sock < 0) { // err catching - WHEN SOCKET CANNOT BE OPENED
    perror ("Error opening channel");
    close(sock);
    exit(1);
  } 
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { // err catching - WRONG URL CASE
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  // the given output of the 'getaddrinfo' func: 
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Resolving %s ... the address is %s\n", hostname, hostaddr); // hostname + ip address
  
  bzero(&cli_name, sizeof(cli_name)); // reset values to avoid strange values
  cli_name.sin_family = AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used  
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); // WHERE TO CONNECT (SERVER IP)
  cli_name.sin_port = htons(port); // set port using the given port

  printf("Connerting to %s | %s ...", hostname, hostaddr);

  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) { // err catching - WHEN IP IS UNKNOWN WE CANNOT CONNECT
    perror ("Error establishing communications");
    close(sock);
    exit(1);
  }
  printf("connected.\n");
  char http_proto[LINE_SIZE]; // string for the http protocol
  sprintf(http_proto, "GET %s HTTP/1.0\nHOST:%s\n\n", url, hostname); // store the http protocol into http_proto and print it
  printf("HTTP request sent, awaiting response... \n");
  send(sock, http_proto, strlen(http_proto), 0); // send the request through the socket
  int writer = write(sock, http_proto, strlen(http_proto)); // write the html content into the socket
  if (writer < 0) { // err catching - WHEN CANNOT WRITING TO SOCKET 
    perror("Error in writing into the socket");
    close(sock);
    exit(1);
  }
  
  char read_buffer[LINE_SIZE];
  while ((read(sock, &read_buffer, sizeof(read_buffer))) > 0) { // read each line from the socket
    printf("%s", read_buffer); // print each line read from the socket
  }
  fflush(stdout); // flushes out the contents of an output stream
  printf("\nExiting now.\n");

  close(sock); 
  exit(0); 
} 
