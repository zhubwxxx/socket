#ifndef UTIL_H
#define UTIL_H

class Util {
public:
    static int setsockPara(int fd);
    static int setsockNonblock(int fd);

};

#endif