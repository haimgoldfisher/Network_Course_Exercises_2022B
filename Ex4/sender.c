#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define SERVER_PORT 7890
#define IP "127.0.0.1"
void sendFile(int);

int main()
{
    printf("staring sender\n");
    for (int i = 0; i < 5; i++)
    {
        printf("iteration No. %d\n",i+1);
        sendFile(1); // RENO
        sendFile(0); // CUBIC
    }
    return 0;
}

void sendFile(int flag)
{
    char buf[1024];
    socklen_t len;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("failed to open socket");
        exit(1);
    } 
    if (flag == 1)
    {
        strcpy(buf, "reno");
        len = strlen(buf);
        if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) 
        {
            perror("setsockopt"); 
            exit(1);
        }
    }
    len = sizeof(buf); 
    if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) 
    {
        perror("getsockopt"); 
        exit(1); 
    } 
    // Connection to Measure

    struct sockaddr_in serverAddress;
    memset(&serverAddress,0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT); // short, network byte order
    serverAddress.sin_addr.s_addr= inet_addr(IP);

    if (inet_pton(AF_INET, IP, &(serverAddress.sin_addr)) < 1) // 0 or -1
    {
        perror("inet_pton");
        exit(1);
    }
    if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("failed to connect");
        exit(1);
    }
    printf("Successfull connection!\n");
    FILE *myFile = fopen("1mb.txt", "r");
    if (myFile == NULL)
    {
        perror("couldn't find the file");
        exit(1);
    }
    int fileData;
    while((fileData = fread(buf, 1, sizeof(buf), myFile)) > 0)
    {
        send(sock, buf, fileData, 0);
    }
    close(sock); 
}
