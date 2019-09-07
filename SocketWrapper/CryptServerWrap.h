#ifndef CRYPTSERVERWRAP_H
#define CRYPTSERVERWRAP_H


#include "../SocketWrapper/ServerWrap.h"


class cryptServer : public ServerSock{
    public:
        cryptServer(const char * addr, uint16_t port) : ServerSock(addr, port);
        ~cryptServer();
};




#endif // CRYPTSERVERWRAP_H

