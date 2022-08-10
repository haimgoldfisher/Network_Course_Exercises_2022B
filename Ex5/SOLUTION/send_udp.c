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
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);// create a UDP socket
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */ // erases sizeof(dest) bytes in memory starting from &dest
  hostptr = gethostbyname(argv[1]); //copies the hostnet for the given host name
  dest.sin_family = (short) AF_INET;//declare type of address (AF_INET means ip and port numbers)
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);//copies data in hostptr to &dest.sin_addr
  dest.sin_port = htons((u_short)0x3333);//converts from host byte to network byte

  msgbuf.head = '<';// part of messege
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */ //same as row 22, but for integers (in this case for the process id)
  msgbuf.tail = '>';//part of messege

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));// send data

  return 0;
}
