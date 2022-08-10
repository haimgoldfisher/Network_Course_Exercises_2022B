//
// Created by bravo8234 on 11/07/2022.
//
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char * argv[])
{
    struct {u_long num;}msg;
    struct hostent *hostnet = gethostbyname(argv[1]);
    int sockFD = socket(AF_INET,SOCK_DGRAM,0);//create udp socket

    struct sockaddr_in dest;//set up destination socket
    bzero((char *)&dest, sizeof (dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons((u_short)0x3333);
    bcopy(hostnet->h_addr, (char *)&dest.sin_addr,hostnet->h_length);

    u_long start = 1;
    for(;;)
    {
        msg.num = start;
        printf("sending %ld\n",start);
        sendto(sockFD,&msg,sizeof(msg),0,(struct sockaddr*)&dest,sizeof (dest));//sending message to dest
        start++;
        sleep(1);
    }

}