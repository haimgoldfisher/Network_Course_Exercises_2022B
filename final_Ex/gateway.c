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

int main(int argc, char *argv[])
{
  int socket_source, socket_sink, cc, fsize; // for the sockets (in & out) & reciver
  struct sockaddr_in  dest, s_in, from; // for the listener and the 'dest' sockets address
  struct { char head; u_long  body; char tail;} msg; // the massage / datagram
  struct hostent *hostptr; // ptr to the host name (from the command line)
  socket_sink = socket (AF_INET, SOCK_DGRAM, 0); // open a new UDP socket (without stating the protocol and returning its ptr) using port P+1 (sender)
  // at this moment, dest is reset to zero until someone sends a message
  bzero((char *) &dest, sizeof(dest));  // set all values in the buffer to zero FOR AVOIDING WRONG VALUES 
  dest.sin_family = (short)AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  u_short port; // for holding the port number
  if (argv[2] != NULL) { // if port number is given by the command line
    port = (u_short)atoi(argv[2]); // using atoi() for scanning the port number from the command line
  }
  else { // default case:
  
    port = (u_short)STD_PORT; // use port 3333 as default port
  }
  dest.sin_port = htons(port + 1); // the dest port is P+1 as required
  hostptr = gethostbyname(argv[1]); // it takes the host name from the command line
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); // it copies the address from hostptr into dest
  socket_source = socket (AF_INET, SOCK_DGRAM, 0); // opens a new socket using port P (receiver)
  bzero((char *) &s_in, sizeof(s_in));  // set all values in the buffer to zero FOR AVOIDING WRONG VALUES 
  s_in.sin_family = (short)AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  s_in.sin_port = htons(port); // the receiver port is P as required
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); // when we don't know the IP address of our machine, we can use the special IP address INADDR_ANY

  bind(socket_source, (struct sockaddr *)&s_in, sizeof(s_in)); // for the listening socket (with the address of source)
  printf("Waiting for datagrams...\n");
  float rand_value, threshold;
  threshold = 0.5; // the random value need to be greater than 0.5
  srandom(9999); // random seed

  for(;;) // this loop will continue until we kill the process (Ctrl+C) - waiting until gets a datagram from source 
  {
    // A message reciever:
    fsize = sizeof(from);
    cc = recvfrom(socket_source,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // waiting for another incoming datagram - WAITING CUBE (using port P)
    rand_value = ((float)random())/((float)RAND_MAX); // between [0,1]
    printf("Random value is: %f", rand_value); // FOR SELF CHECK & help to know if the msg should be sent or not
    // printsin( &from, "recv_udp: ", "Packet from:"); // so we can know the port & IP of the client who sent the message (HIS HEADER)
    if(rand_value > threshold)
    {
      printf(" (> 0.5), Sending the message %c%ld%c Port: %d -> %d\n",msg.head, msg.body,msg.tail, ntohs(s_in.sin_port), ntohs(dest.sin_port));
      sendto(socket_sink,&msg,sizeof(msg),0,(struct sockaddr *)&dest, sizeof(dest)); // SEND IT (using port P+1)
    }
    else // if rand_value < 0.5
    {
      printf(" (< 0.5), Not sending the message %c%ld%c \n",msg.head, msg.body,msg.tail); // DONT SEND IT
    }
    fflush(stdout); // flushes out the contents of an output stream
  }

  return 0;
}
