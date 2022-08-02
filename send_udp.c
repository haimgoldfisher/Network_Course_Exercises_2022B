#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int socket_fd; // for the socket we are going to open
  struct sockaddr_in  dest; // adress from the socked dest
  struct hostent *hostptr; // ptr for the host name details
  struct { char head; u_long body; char tail; } msgbuf; // the data

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // it creates a new UDP socket (without stating the protocol and returning its ptr)
  bzero((char *) &dest, sizeof(dest)); // set all values in the buffer to zero FOR AVOIDING WRONG VALUES 
  hostptr = gethostbyname(argv[1]); // it takes the host name from the command line
  dest.sin_family = (short) AF_INET; // contains the address family, which is always AF_INET when TCP or UDP is used
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); // it copies the data from hostptr into dest
  dest.sin_port = htons((u_short)0x3333); // converting to TCP/IP network bytes

  msgbuf.head = '<'; // first char of the msg
  msgbuf.body = htonl(getpid()); // the body of the message, getpid() - it gets the process ID of the process that calls that function
  msgbuf.tail = '>'; // last char of the msg

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest)); // it sends the data from the msg

  return 0;
}
