#include "server_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include <string>

#include "common/util.h"

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
    if (pthread_create(&pid, NULL, workThread, server) != 0) {
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
        delete server;
        return NULL;
    }
    server->clientNum = 0;
    ClientInfo *client;
    for (int i = 0; i < MAX_CLIENT_NUM; i++) {
        client = &server->clients[i];
        client->port = -1;
        client->connfd = -1;
        client->recvBuf = (char *)malloc(MAX_CLIENT_BUF_SIZE);
        if (client->recvBuf == NULL) {
            perror("malloc()");
            delete server;
            return NULL;
        }
        client->recvLen = 0;
        client->sendBuf = NULL;
        client->sendLen = 0;
    }
    server->maxfd = server->fd;
    FD_ZERO(&server->fd_rw);
    FD_SET(server->fd, &server->fd_rw);
    
    return server;
}

int ServerSocket::bindServer(const char *addr, uint16_t port)
{
    // 创建socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket() error");
        return -1;
    }
    // bind
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    // sa.sin_addr.s_addr = inet_addr(addr);
    if (addr && strcmp(addr, "\0") != 0 && inet_aton(addr, &sa.sin_addr) == 0) {
        perror("inet_aton()");
        close(fd);
        return -1;
    }
    if (bind(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind() error");
        close(fd);
        return -1;
    }

    //设置地址可重用
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("setsockopt()");
        close(fd);
        return -1;
    }

    if (listen(fd, LISTENQU) < 0) {
        perror("listen() error");
        close(fd);
        return -1;
    }
    return fd;
}

void* ServerSocket::workThread(void *arg)
{
    ServerInfo *server = (ServerInfo*)arg;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000;

    while (true) {
        doAccept(server);
        nanosleep(&ts, NULL);
    }

}

int ServerSocket::doAccept(ServerInfo *server)
{
    FD_ZERO(&server->fd_read);
    FD_ZERO(&server->fd_write);
    server->fd_read = server->fd_rw;
    server->fd_write = server->fd_rw;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    ClientInfo *client = NULL;
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int ret = select(server->maxfd+1, &server->fd_read, &server->fd_write, NULL, &tv);
    if (ret < 0) {
        perror("select()");
        return -1;
    } else if (ret == 0) {
        return 0;
    }
    // 是否有新连接
    if (FD_ISSET(server->fd, &server->fd_read)) {
        int connfd = accept(server->fd, (struct sockaddr*)&clientAddr, &addrLen);
        if (connfd < 0) {
            perror("accept()");
            return -1;
        }
        if (Util::setsockPara(connfd) < 0) {
            return -1;
        }
        if (Util::setsockNonblock(connfd) < 0) {
            return -1;
        }            
        bool find = false;
        for (int i = 0; i < MAX_CLIENT_NUM; i++) {
            client = &server->clients[i];
            if (client->connfd == -1) {
                client->connfd = connfd;
                strncpy(client->addr, inet_ntoa(clientAddr.sin_addr), MAX_ADDR_LEN-1);
                client->port = ntohs(clientAddr.sin_port);
                if (connfd > server->maxfd) {
                    server->maxfd = connfd;
                }
                server->clientNum++;  
                FD_SET(client->connfd, &server->fd_rw);                  
                find = true;
                break;
            }
        }
        if (find) {
            printf("recv a client, addr = %s, port = %d, connfd = %d\n", client->addr, client->port, client->connfd);
        } else {
            printf("the client is too much!!!\n");
            close(connfd);
        }
    }
    // 轮询每个已连接客户端的connfd是否有数据
    for(int i = 0; i < MAX_CLIENT_NUM; i++) {
        client = &server->clients[i];
        if (client->connfd == -1) {
            continue;
        }
        if (FD_ISSET(client->connfd, &server->fd_read)) {
            if (readMsg(client) < 0) {
                close(client->connfd);
                FD_CLR(client->connfd, &server->fd_rw);
                updateConnfd(server, client->connfd);
                client->connfd = -1;
                server->clientNum--;
                printf("readMsg() error, close the socket, ip: %s, port: %d, fd: %d\n", client->addr, client->port, client->connfd);
            }
        }        
    }

}

int ServerSocket::readMsg(ClientInfo *client)
{   
    client->recvLen = read(client->connfd, client->recvBuf, MAX_CLIENT_BUF_SIZE);
    if (client->recvLen < 0) {
        perror("read()");
        return -1;
    }
    return 0;
}

void ServerSocket::updateConnfd(ServerInfo *server, int fd)
{
    ClientInfo *client = NULL;
    int maxFd = 0;
    if (fd >= server->maxfd) {
        for (int i = 0; i < MAX_CLIENT_NUM; i++) {
            client = &server->clients[i];
            if (client->connfd > maxFd) {
                maxFd = client->connfd;                
            }
        }
        server->maxfd = maxFd;
    }    
}