#ifndef CRYPTCLIENTWRAP_H
#define CRYPTCLIENTWRAP_H


#include "../SocketWrapper/ClientWrap.h"


class cryptClient : public ClientSock{
    public:
        cryptClient(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port) : ClientSock(from_addr, from_port, to_addr, to_port);
        ~cryptClient();
};




#endif // CRYPTCLIENTWRAP_H

