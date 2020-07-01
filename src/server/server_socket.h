#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <stdint.h>
#include "common/config.h"

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();
    int startServer();

    ServerInfo* initServerInfo(const char *addr, uint16_t port);
    int bindServer(const char *addr, uint16_t port);
    static void* procData(void *arg);

private:

};


#endif // SERVERSOCKET_H