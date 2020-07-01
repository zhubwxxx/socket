#include "server_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include <iostream>
#include <string>

using namespace std;

ServerSocket::ServerSocket()
{

}

ServerSocket::~ServerSocket()
{

}

int ServerSocket::startServer()
{
    string addr = "127.0.0.1";
    ServerInfo *server = initServerInfo(addr.c_str(), LISTEN_PORT);
    if (server == NULL) {
        printf("initServerInfo() error.\n");
        return -1;
    }
    pthread_t pid;
    if (pthread_create(&pid, NULL, procData, server) != 0) {
        perror("pthread_create()");
        return -1;
    }
    pthread_detach(pid);
    printf("start server socket success.\n");
    return 0;
}

ServerInfo* ServerSocket::initServerInfo(const char *addr, uint16_t port)
{
    ServerInfo *server = new ServerInfo;
    strncpy(server->addr, addr, MAX_ADDR_LEN);
    server->port = port;
    server->fd = bindServer(addr, port);
    if (server->fd < 0) {
        printf("bindServer() error.\n");
        return NULL;
    }
    ClientInfo *client;
    for (int i = 0; i < MAX_CLIENT_NUM; i++) {
        // client = &server->clients[i];

    }
    for (int i = 0; i < MAX_CLIENT_NUM; i++) {
        
    }
    server->maxfd = server->fd;
    
    return server;
}

int ServerSocket::bindServer(const char *addr, uint16_t port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket() error");
        return -1;
    }
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(addr);
    if (bind(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind() error");
        return -1;
    }
    if (listen(fd, LISTENQU) < 0) {
        perror("listen() error");
        return -1;
    }
    return fd;
}

void* ServerSocket::procData(void *arg)
{

}