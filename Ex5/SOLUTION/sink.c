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
#include <time.h>
#include <stdlib.h>
#include <arpa/inet.h>
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

int main(int argc, char* argv[])
{
    int cc,fsize;
    struct sockaddr_in listen,recv;
    struct {u_long num;}msg;
    int sockFD = socket(AF_INET,SOCK_DGRAM,0);//create udp socket

    bzero((char *)&listen,sizeof(listen));//reset memory in the address of the socket

    listen.sin_family = (short)AF_INET;// IP + port
    listen.sin_addr.s_addr = htonl(INADDR_ANY);//recives from every internal interface
    listen.sin_port = htons(((u_short)0x3334));//set port

    printsin( &listen, "RECV_UDP", "Local socket is:");
    fflush(stdout);//writes to or updates the standard output

    bind(sockFD, (struct sockaddr* )&listen ,sizeof(listen));// bind socket and address
    int counter = 0;// count how many messegas got
    for(;;)
    {
        fsize = sizeof(recv);
        cc = recvfrom(sockFD,&msg,sizeof(msg),0,(struct sockaddr *)&recv,&fsize);//recvive messege from socket
        printsin(&recv,"recv_udp:","from:");//print info of socket
        printf("Got data :: %ld\n",msg.num);// prints the process id gotten from the socket
        counter++;
        if(counter%100 == 0)//print how many messages got and success rate
        {
            printf("recevied %d messages\n",counter);
            printf("success rate: %f\n",((float )counter)/((float )msg.num));
        }
    }
    return 0;
}