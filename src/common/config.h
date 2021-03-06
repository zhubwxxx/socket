#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <sys/select.h>

const int LISTEN_PORT = 11000; // 监听端口
const int MAX_CLIENT_NUM = 32; // 最多链接客户端
const int MAX_CLIENT_BUF_SIZE = 1024*1024;
const int MAX_RECV_BUF_SIZE = 1024*1024; // 最大接收buf
const int MAX_SEND_BUF_SIZE = 1024*1024; // 最大发送buf
const int MAX_ADDR_LEN = 32; // ip地址长度
const int LISTENQU = 32;
const int MAX_BUF_SIZE = 512;

typedef struct {
    char addr[MAX_ADDR_LEN]; // 客户端地址
    uint16_t port; //端口
    int connfd; 
    char *recvBuf; //接收缓存区
    int recvLen; 
    char *sendBuf; // 发送缓存区
    int sendLen;
} ClientInfo;

typedef struct {
    char addr[MAX_ADDR_LEN]; // 服务端地址
    uint16_t port; //端口
    int fd;
    int maxfd;    
    fd_set fd_rw;    
    fd_set fd_read;
    fd_set fd_write;
    int clientNum;
    ClientInfo clients[MAX_CLIENT_NUM];
} ServerInfo;

#endif // CONFIG_H