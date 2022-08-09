#include <stdio.h>
#include <string.h>
int main(void) {
        
    // const char url[] = "http://192.168.0.2:8888/servlet/rece";
    const char url[] = "https://www.yahoo.com:8888/does-not-exist";
    // const char url[] = "https://www.yahoo.com:30/";
    char host[100];
    int port = 80;
    char path[100];
    // sscanf(url, "http://%99[^:]:%99d/%99[^\n]", host, &port, path);
    sscanf(url, "%*[^:]%*[:/]%[^:]:%d%s", host, &port, path);
    if (port = 80)
    {
        if (strlen(path) == 0)
        {
            sscanf(host, "%*[^/]%[^:]:", path);
            sscanf(host, "%[^/]", host);
        }
    }
    printf("ip = \"%s\"\n", host);
    printf("port = \"%d\"\n", port);
    printf("page = \"%s\"\n", path);
    return 0;
}