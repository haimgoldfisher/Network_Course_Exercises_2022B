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
#include <stdlib.h>

/* the purpose of this function is to print the PORT and the IP ID of the sender */

void printsin(struct sockaddr_in *sin, char *pname, char *msg) 
{
  printf("%s\n", pname);
  printf("%s ", msg);
  printf("ip= %s, ", inet_ntoa(sin->sin_addr)); // IP in dotted-decimal notation
  printf("port= %d \n", sin->sin_port); // host in integer type 
}

int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize; // for the socket & reciver
  struct sockaddr_in  s_in, from; // for the listener and the 'from' sockets address
  struct { char head; u_long  body; char tail;} msg;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // open a new UDP socket (without stating the protocol and returning its ptr)
  // at the start, everything is zero until someone sends a message
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */ // FOR AVOIDING WRONG VALUES 

  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333 + 1); // listen to P+1 as requred

  printsin( &s_in, "from_UDP", "Local socket is:"); // first print, with zeroes (to avoid strange values)
  fflush(stdout); // flushes out the contents of an output stream

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
  u_long messages = 0;
  for(;;) { // this loop will continue until we kill the process (Ctrl+C) - waiting until gets a datagram from gateway
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // BLOCKING CALL until someone sends a message - WAITING CUBE
    printsin( &from, "from_udp: ", "Packet from:"); // so we can know the port & IP of the client who sent the message (HIS HEADER)
    printf("Got data :%c%ld%c\n",msg.head, msg.body,msg.tail);
    messages++;
    // fflush(stdout);
    u_long missed_messages = msg.body - messages; // the highest num of message MINUS num of sented messages = unsent messages  
    printf("%ld of %ld messages were sent, %ld messages were not sent. \n", msg.body-missed_messages, msg.body, missed_messages);
  }
  
  return 0;
}
