
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

#define MAX_SEND_RECV 1024
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in cli_name;
    int count = 0;
    int value;
    int port = 0;
    printf("Client is alive and establishing socket connection.\n");

    //parsing the url
    struct addrinfo *res;
    char *url = argv[1];//getting commend line argument (URL)
    bzero(&cli_name, sizeof(cli_name));

    int hostStart = 7;

    int lastSlash = 0; // searching for last slash (start of path)
    for(int i =0; i < strlen(url);i++){
        if(url[i] == '/')
        {
            lastSlash = i;
        }
    }
    //looking for the length of the hostname
    while (url[hostStart] != '/' && url[hostStart] != ':') {
        count++;
        hostStart++;
    }

    // checking if there is a port with the URL
    if (url[count + 7] != ':') {
        port = 80;

    } else {
        int portStart = 8 + count;
        int portCount = 0;//counting length of port
        while (url[portStart] != '/') {
            portCount++;
            portStart++;
        }
        char *portChar = (char *) malloc((portCount + 1) * sizeof(char));//allocating space for port
        for (int i = 0; i < portCount; i++) {
            portChar[i] = url[i + (portStart-portCount)];
        }
        portChar[portCount + 7] = '\0';
        port = strtol(portChar, NULL, 10);//parsing port to number
        free(portChar);
    }

    int countPath = 0;
    int current = lastSlash;
    //counting length of path
    while(current+1 < strlen(url))
    {
        current++;
        countPath++;
    }
    char* pathChar = (char *) malloc((countPath + 1) * sizeof(char));//allocating space for path
    for(int i = 0; i < countPath;i++)
    {
        pathChar[i] = url[i + lastSlash +1];
    }
    pathChar[lastSlash+countPath] = '\0';

    char *hostname = (char *) malloc((count + 1) * sizeof(char));//allocating space for host name
    for (int i = 0; i < count; i++) {
        hostname[i] = url[i + 7];
    }
    hostname[count + 7] = '\0';

    //getting the IP address by the host
    struct hostent *host = gethostbyname(hostname);
    if (getaddrinfo(host->h_name, 0, 0, &res) != 0) {
        printf("error");
        exit(1);
    }
    struct sockaddr_in *saddr = (struct sockaddr_in *) res->ai_addr;
    char *ip_address = inet_ntoa(saddr->sin_addr);

    //setting socket up
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(ip_address);//need to write the right IP address
    cli_name.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);//creating TCP socket
    if (sock < 0)//checking validity
    {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }

    if(connect(sock,(struct sockaddr*)&cli_name,sizeof (cli_name)) == -1){
        perror("connect");
        exit(1);
    }
    char send[MAX_SEND_RECV +1];
    char recv[MAX_SEND_RECV +1];
    sprintf(send,"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n",pathChar,hostname);//parsing the request
    if(write(sock,send,strlen(send)) >= 0)
    {
        size_t n;
        while((n = read(sock,recv,MAX_SEND_RECV)) > 0)//reading the response
        {
            recv[n] = '\0';
            if(fputs(recv,stdout) == EOF)//printing the response
            {
                printf("fputs() error");
            }
        }
    }
    printf("Exiting now.\n");
    free(pathChar);
    free(hostname);
    close(sock);
    return 0;

}
