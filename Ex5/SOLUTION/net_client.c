
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIM_LENGTH 10
#define IP_ADDRESS "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in cli_name;
    int count;
    int value;

    printf("Client is alive and establishing socket connection.\n");

    //setting up IP adress of hostname
    struct addrinfo *res;
    char *hostname = argv[1];//getting commend line argument
    if (getaddrinfo(hostname, NULL, NULL, &res) != 0)//trying to copy the address of hostname to res
    {
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }
    //moving the address to a string
    struct sockaddr_in *saddr = (struct sockaddr_in *) res->ai_addr;
    char *ip_address = inet_ntoa(saddr->sin_addr);
    printf("The IP for hostname %s is %s\n", hostname, ip_address);

    sock = socket(AF_INET, SOCK_STREAM, 0);//creating TCP socket
    if (sock < 0)//checking validity
    {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }


    //setting up socket after it exists
    bzero(&cli_name, sizeof(cli_name));
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(ip_address);//need to write the right IP address
    cli_name.sin_port = htons(
            PORT);//need to check that PORT between server and client match, otherwise connection refused


    if (connect(sock, (struct sockaddr *) &cli_name, sizeof(cli_name)) < 0)//trying to connect to a socket
    {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }


    for (count = 1; count <= SIM_LENGTH; count++) {
        read(sock, &value, 4);//reading data from socket
        printf("Client has received %d from socket.\n", value);
    }

    printf("Exiting now.\n");

    close(sock);
    exit(0);

} 
