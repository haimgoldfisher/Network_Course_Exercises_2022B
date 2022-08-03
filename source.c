#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd; // for the socket we are going to open
  struct { char head; u_long body; char tail; } msg; // the msg / datagram
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // it creates a new UDP socket (without stating the protocol and returning its ptr)
  struct sockaddr_in  dest; // the dest socket - where we send the msg
  bzero((char *) &dest, sizeof(dest)); // set all values in the buffer to zero FOR AVOIDING WRONG VALUES 
  dest.sin_family = AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  dest.sin_port = htons((u_short)0x3333);  // converting to TCP/IP network bytes
  struct hostent *hostptr; // ptr for the host name details
  hostptr = gethostbyname(argv[1]);  // it takes the host name from the command line
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); //it copies the data from hostptr into dest

  /* each msg in <n> form (n=1,2,3...) :*/

  msg.head = '<';
  msg.body = 0; // each datagram carries in it's body an integer number
  msg.tail = '>';
  u_long index = 0; // for first message body/index
  
  for(;;) // this loop will continue until we kill the process (Ctrl+C)
  {
    index++; // increments the index for next msg 
    msg.body = index; // so every msg has it's index
    // sending our msg to the dest socket
    printf("Send Data: %c%ld%c\n",msg.head, msg.body,msg.tail);
    sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&dest, sizeof(dest)); // send the <n> msg to dest socket
    sleep(1); // sleep for one sec as required
  }
  
  return 0;
}
