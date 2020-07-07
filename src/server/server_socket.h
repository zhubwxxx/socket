#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <stdint.h>
#include "common/config.h"

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();
    int startServer();

private:
    ServerInfo* initServerInfo(const char *addr, uint16_t port);
    int bindServer(const char *addr, uint16_t port);
    static void* workThread(void *arg);
    static int doAccept(ServerInfo *server);
    static int readMsg(ClientInfo *client);
    static void updateConnfd(ServerInfo *server, int fd);

};


#endif // SERVERSOCKET_H