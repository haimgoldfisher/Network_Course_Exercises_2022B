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
#define STD_PORT 3333 // default port if port number is not given from the command line

/* the purpose of this function is to print the PORT and the IP ID of the sender */

void printsin(struct sockaddr_in *sin, char *pname, char *msg) 
{
  printf("%s\n", pname);
  printf("%s ", msg);
  printf("ip= %s, ", inet_ntoa(sin->sin_addr)); // IP in dotted-decimal notation
  printf("port= %d \n", ntohs(sin->sin_port)); // port in integer type 
}

int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize; // for the socket & reciver
  struct sockaddr_in  s_in, from; // for the listener and the 'from' sockets address
  struct { char head; u_long  body; char tail;} msg; // the msg / datagram

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // open a new UDP socket (without stating the protocol and returning its ptr)
  // at the start, everything is zero until someone sends a message
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */ // FOR AVOIDING WRONG VALUES 

  s_in.sin_family = (short)AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); // WILDCARD: when we don't know the IP address of our machine, we can use the special IP address INADDR_ANY
  u_short port; // for holding the port number
  if (argv[1] != NULL) { // if port number is given by the command line
    port = (u_short)atoi(argv[1]); // using atoi() for scanning the port number from the terminal
  }
  else {
    port = (u_short)STD_PORT ; // use port 3333 as default port
  }
  s_in.sin_port = htons(port + 1);  // listen to P+1 as required
  printsin( &s_in, "from_UDP", "Local socket is:"); // first print, with zeroes (to avoid strange values)
  fflush(stdout); // flushes out the contents of an output stream
  
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
  u_long messages = 0; // counter for the num of message that it received

  for(;;) { // this loop will continue until we kill the process (Ctrl+C) - waiting until gets a datagram from gateway
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // BLOCKING CALL until someone sends a message - WAITING CUBE
    printsin(&from, "from_udp: ", "Packet from:"); // so we can know the port & IP of the client who sent the message (HIS HEADER)
    printf("Got data :%c%ld%c\n",msg.head, msg.body,msg.tail);
    messages++;
    u_long missed_messages = msg.body - messages; // the highest num of message MINUS num of sented messages = unsent messages  
    printf("%ld of %ld messages were sent, %ld messages were not sent. \n", msg.body-missed_messages, msg.body, missed_messages); // temp conclusion
    fflush(stdout); // flushes out the contents of an output stream
  }
  
  return 0;
}
