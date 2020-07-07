#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <stdint.h>
#include "common/config.h"

class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();
    int startClient();

private:
    ClientInfo* initClientInfo(const char *addr, uint16_t port);
    int initSockFd(const char *addr, uint16_t port);
    static void* workThread(void *arg);
    static int procData(ClientInfo *client); 
    static int sendMsg(int fd, char *buf, int len);
};


#endif // CLIENTSOCKET_H