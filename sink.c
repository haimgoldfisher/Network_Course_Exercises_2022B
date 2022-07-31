#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

/*
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  -- port: sin->sin_port (host integer type) 
  -- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}
*/

// the purpose of this function is to print the port and the ip of the sender (+ the message)
void printsin(struct sockaddr_in *sin, char *pname, char *msg) 
{
  unsigned short port = sin->sin_port;
  unsigned long ip = inet_addr(sin);
  printf("%s\n", pname);
  printf("%s: ", msg);
  printf("ip= %d", ip); // IP in dotted-decimal notation
  printf("port= %d, ", port); // host integer type 
  printf("\n");
}


int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  // at the start, everything is zero until someone sends a message
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333);

  printsin( &s_in, "RECV_UDP", "Local socket is:"); // first print, with zeroes (to avoid strange values)
  fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // BLOCKING CALL until someone sends a message - WAITING CUBE
    printsin( &from, "recv_udp: ", "Packet from:"); // so we can know the port & IP of the client who sent the message (HIS HEADER)
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);
  }
  
  return 0;
}
