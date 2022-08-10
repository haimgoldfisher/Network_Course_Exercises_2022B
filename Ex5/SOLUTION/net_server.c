#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define PORT 10000
#define SIM_LENGTH 10

int main(void) {
    int sock;
    int connect_sock;
    struct sockaddr_in serv_name;
    size_t len;
    int count;

    sock = socket(AF_INET, SOCK_STREAM, 0); //creating TCP socket

    //set up socket settings
    bzero(&serv_name, sizeof(serv_name));
    serv_name.sin_family = AF_INET;
    serv_name.sin_port = htons(PORT);

    //binding socket with address
    bind(sock, (struct sockaddr *) &serv_name, sizeof(serv_name));

    //listening till 1 connection
    listen(sock, 1);

    len = sizeof(serv_name);

    //accepting 1 connection
    connect_sock = accept(sock, (struct sockaddr *) &serv_name, &len);

    for (count = 1; count <= SIM_LENGTH; count++) {
        write(connect_sock, &count, 4);//writing into socket
        printf("Server has written %d to socket.\n", count);
    }

    //closing sockets
    close(connect_sock);
    close(sock);

}
