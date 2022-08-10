#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#define SERVER_PORT 7890
#define SIZE 1024

double reciever(int);

int main()
{
    printf("staring measure\n");
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        perror("failed to open socket");
        exit(1);
    } 
    int yes = 1; 
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    { 
	    perror("setsockopt"); 
	    exit(1);
    }
    struct sockaddr_in serverAddress;
    memset(&serverAddress,0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(SERVER_PORT); // short, network byte order
    if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(sock, 100) == -1) 
    {
        printf("listen");
        close(sock);
        exit(1);
    }
    printf("Listening to port %d\n",SERVER_PORT);
    double avg_reno = 0;
    double avg_cubic = 0;
    for (int i = 0; i < 5; i++)
    {
        printf("iteration No. %d\n",i+1);
        double curr_reno = reciever(sock); // RENO
        double curr_cubic = reciever(sock); // CUBIC
        printf("the current Reno Time is %f\n",curr_reno);
        printf("the current Cubic Time is %f\n",curr_cubic);
        avg_reno += curr_reno;
        avg_cubic += curr_cubic;
    }
    avg_reno = avg_reno/5;
    avg_cubic = avg_cubic/5;
    printf("Average:\n");
    printf("the AVG Reno Time is %f\n",avg_reno);
    printf("the AVG Cubic Time is %f\n",avg_cubic);
    printf("closing socket\n");


    close(sock);
    return 0;
}

double reciever(int sock)
{
    struct sockaddr_in clientAdrr;
    socklen_t clientAddrSize = sizeof(clientAdrr);
    memset(&clientAdrr, 0, sizeof(clientAdrr));

    clientAddrSize = sizeof(clientAdrr);

    int newSock = accept(sock, (struct sockaddr *)&clientAdrr, &clientAddrSize);
    if(newSock == -1) {
        printf("listen");
        close(sock);
        exit(0);
    }
    char buff[SIZE];
    // clock_t startTime = clock();
    struct timespec time1, time2;
    clock_gettime(CLOCK_REALTIME, &time1);
    while(recv(newSock, buff, SIZE, 0) != 0)
    {
        bzero(buff, SIZE);
    }
    clock_gettime(CLOCK_REALTIME, &time2);
    // clock_t endTime = clock();
    // double time =((double)(endTime - startTime))/CLOCKS_PER_SEC;
    return (time2.tv_sec - time1.tv_sec) + (time2.tv_nsec - time1.tv_nsec) / 1000000000.0;;

}
