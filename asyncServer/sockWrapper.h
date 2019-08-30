#ifndef SOCK_WRAPPER_H
#define SOCK_WRAPPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <iostream>


using std :: cerr;
using std :: cout;
using std :: endl;


class sockWrapper{
    public:
        sockWrapper(const char * addr, uint16_t port);
        ~sockWrapper();
        void _bind();
        void _recv(char * buffer);
        void _listen(size_t number);
        void _accept(uint16_t port);

        uint16_t port_num = 0;
        int sock_id = 0;
        struct sockaddr_in addrStruct;
        socklen_t opt = 1;
};


sockWrapper :: sockWrapper(const char * addr, uint16_t my_port){
    this->sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sock_id < 0) cerr << "Socket creation error";
    if(setsockopt(this->sock_id, IPPROTO_TCP, SO_REUSEADDR, (void*)opt, sizeof(socklen_t)) < 0 ) cerr <<  "Error of setting option";
    this->port_num = my_port;
    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = inet_addr(addr);
    this->addrStruct.sin_port = htons(my_port);
}

void sockWrapper :: _bind(){
    bind(this->sock_id, (struct sockaddr *)&this->addrStruct, sizeof(this->addrStruct));
    if(this->sock_id < 0) cerr << "Binding error";
    else cout << "Socket created" << endl;
}


void sockWrapper :: _listen(size_t number){
    listen(this->sock_id, number);
}


void sockWrapper :: _accept(uint16_t port){
    this->sock_id = accept(port, (sockaddr*)&this->addrStruct, (socklen_t*)&this->addrStruct);
    if(sock_id) cerr << "Acception error";
}


void sockWrapper :: _recv(char * buffer){
    if(recv(this->sock_id, &buffer, sizeof(buffer), 0) < 0) cerr << "Receiving error";
}


sockWrapper :: ~sockWrapper(){
    shutdown(this->sock_id, 2);
    close(this->sock_id);
    memset(&this->addrStruct, 0, sizeof(this->addrStruct));
}


class cryptoWrapper : public sockWrapper{
    public:
        using sockWrapper :: sockWrapper;
        cryptoWrapper();
        ~cryptoWrapper();
};

#endif //SOCK_WRAPPER_H
