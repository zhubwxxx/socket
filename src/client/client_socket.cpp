#include "client_socket.h"

#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "common/util.h"

using namespace std;

ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

int ClientSocket::startClient()
{
    string addr = "127.0.0.1";
    ClientInfo *client = initClientInfo(addr.c_str(), LISTEN_PORT);
    if (client == NULL) {
        printf("initClientInfo() error.\n");
        return -1;
    }
    pthread_t pid;
    if (pthread_create(&pid, NULL, workThread, client) != 0) {
        perror("pthread_create()");
        return -1;
    }
    pthread_detach(pid);
    printf("start client socket success.\n");

    return 0;
}

ClientInfo* ClientSocket::initClientInfo(const char *addr, uint16_t port)
{
    ClientInfo *client = new ClientInfo;
    strncpy(client->addr, addr, MAX_ADDR_LEN-1);
    client->port = port;
    client->connfd = initSockFd(addr, port);
    if (client->connfd < 0) {
        printf("initSockFd() error.\n");
        delete client;
        return NULL;
    }

    // Util::setsockNonblock(client->connfd);
    // Util::setsockPara(client->connfd);

    client->sendBuf = (char *)malloc(MAX_CLIENT_BUF_SIZE);    
    if (client->sendBuf == NULL) {
        perror("malloc()");
        delete client;
        return NULL;
    }
    client->sendLen = 0;

    return client;
}

int ClientSocket::initSockFd(const char *addr, uint16_t port)
{
    // 创建socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket() error");
        return -1;
    }
    // connect 
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (addr && strcmp(addr, "\0") != 0 && inet_aton(addr, &sa.sin_addr) == 0) {
        perror("inet_aton()");
        close(fd);
        return -1;
    }
    if (connect(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("connect()");
        close(fd);
        return -1;
    }

    return fd;
}


void* ClientSocket::workThread(void *arg)
{
    ClientInfo *client = (ClientInfo *)arg;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000;

    while (true) {
        procData(client);
        nanosleep(&ts, NULL);
    }

}

int ClientSocket::procData(ClientInfo *client)
{
    char buf[MAX_BUF_SIZE] = { 0 };
    for (int i = 0; i < MAX_BUF_SIZE; i++) {
        buf[i] = i % 2 == 0 ? '0' : '1'; 
    }
    buf[MAX_BUF_SIZE-1] = '\0';

    if (sendMsg(client->connfd, buf, strlen(buf)) < 0) {
        printf("sendMsg() error.\n");
        return -1;
    }
    printf("sendMsg: len=%d, buf=%s\n", strlen(buf), buf);    
    return 0;
}

int ClientSocket::sendMsg(int fd, char *buf, int len)
{
    int nwrite = 0;
    int left = len;
    char *ptr = buf;
    
    while (left > 0) {
        ;
        if ((nwrite = write(fd, ptr, left)) <= 0) {
            if ((nwrite < 0) && (errno == EAGAIN) && (errno == EWOULDBLOCK) && (errno == EINTR)) {
                nwrite = 0;
            } else {
                perror("write()");
                return -1;
            }
        } 
        left -= nwrite;
        ptr += nwrite;
    }
    return 0;
}
