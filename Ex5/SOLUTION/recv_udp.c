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


void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  int port = s->sin_port;// get port num
  char str[INET_ADDRSTRLEN] = {0};
  inet_ntop(AF_INET, &s->sin_addr,str, INET_ADDRSTRLEN);// copy ip address as chars to array
  printf("ip= ");
  for(int i = 0; i < INET_ADDRSTRLEN;i++)
  {
      printf("%c",str[i]);
  }
  printf(" port = %d",port);
  //-- port: sin->sin_port (host integer type)
  //-- IP: sin->sin_addr (IP in dotted-decimal notation)
  printf("\n");
}


int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);// create udp socket

  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */ // delete memory in this space

  s_in.sin_family = (short)AF_INET; // same as in send_udp
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);//bounds the socket to all local interfaces    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333);// same as in send_udp

  printsin( &s_in, "RECV_UDP", "Local socket is:");
  fflush(stdout);//writes to or updates the standard output

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));// assigns the address (struct sockaddr *)&s_in to socket_fd

  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);//recvive messege from socket
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail);// prints the process id gotten from the socket
    fflush(stdout);//writes to or updates the standard output
  }
  
  return 0;
}

