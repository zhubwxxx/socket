#include "util.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/time.h>

#include "config.h"

int Util::setsockPara(int fd)
{
    int bufSize = MAX_RECV_BUF_SIZE;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &bufSize, sizeof(bufSize)) < 0) {
        perror("setsockopt() SO_RCVBUF error");
        return -1;
    }
    bufSize = MAX_SEND_BUF_SIZE;
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufSize, sizeof(bufSize)) < 0) {
        perror("setsockopt() SO_SNDBUF error");
        return -1;
    }
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 50;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt() SO_RCVTIMO error");
        return -1;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt() SO_SNDTIMEO error");
        return -1;
    }

    return 0;
}

int Util::setsockNonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl() F_GETFL error");
        return -1;
    }
    int ret = fcntl(fd, F_SETFL, flags|O_NONBLOCK);
    if (ret < 0) {
        perror("fcntl() F_SETFL error");
        return -1;
    }
    return 0;
}

