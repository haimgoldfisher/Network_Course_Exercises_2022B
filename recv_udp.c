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
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg; // the massage

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // open a new UDP socket (without stating the protocol and returning its ptr)
  // at the start, everything is zero until someone sends a message
  bzero((char *) &s_in, sizeof(s_in));  // set all values in the buffer to zero FOR AVOIDING WRONG VALUES 

  s_in.sin_family = (short)AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); // when we don't know the IP address of our machine, we can use the special IP address INADDR_ANY
  s_in.sin_port = htons((u_short)0x3333); // converting to TCP/IP network bytes

  printsin( &s_in, "RECV_UDP", "Local socket is:"); // first print, with zeroes (to avoid strange values)
  fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); // it checks if port is available

  for(;;) { // while TRUE:
    // A message reciever:
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // BLOCKING CALL until someone sends a message - WAITING CUBE
    printsin( &from, "recv_udp: ", "Packet from:"); // so we can know the port & IP of the client who sent the message (HIS HEADER)
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); // printer for the data which the msg holds
    fflush(stdout);
  }
  
  return 0;
}
