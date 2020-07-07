#include "server_socket.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    ServerSocket server;
    int ret = server.startServer();
    if (ret < 0) {
        printf("start server error!!!\n");
    } else {
        printf("start server success!!!\n");
        while (true) {
            sleep(3);
        }
    }
    return 0;
}