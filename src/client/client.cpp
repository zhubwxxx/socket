#include "client_socket.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    ClientSocket client;
    int ret = client.startClient();
    if (ret < 0) {
        printf("start client error!!!\n");
    } else {
        printf("start client success!!!\n");
        while (true) {
            sleep(3);
        }
    }

    return 0;
}