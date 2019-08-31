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
        sockWrapper(uint16_t port);
        ~sockWrapper();
        void _bind();
        void _recv(char * buffer);
        void _listen(size_t number);
        void _accept(uint16_t port);

        int sock_id = 0;
        struct sockaddr_in addrStruct;
};


sockWrapper :: sockWrapper(const char * addr, uint16_t port){
    this->sock_id = socket(AF_INET, SOCK_STREAM, 0);

    if(this->sock_id < 0) cerr << "Socket creation error" << endl;
    else cout << "Sock created" << endl;

    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = inet_addr(addr);
    this->addrStruct.sin_port = htons(port);
    cout << "Sock initialized " << addr << " " << port << endl;
}

sockWrapper :: sockWrapper(uint16_t port){
    this->sock_id = socket(AF_INET, SOCK_STREAM, 0);

    if(this->sock_id < 0) cerr << "Socket creation error" << endl;
    else cout << "Sock created " << endl;

    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
    this->addrStruct.sin_port = htons(port);
    char * addr = inet_ntoa(this->addrStruct.sin_addr);
    std ::string s(addr);
    cout << "Sock initialized " << s << " " << port << endl;
}

void sockWrapper :: _bind(){
    bind(this->sock_id, (struct sockaddr *)&this->addrStruct, sizeof(this->addrStruct));
    if(this->sock_id < 0) cerr << "Binding error" << endl;
    else cout << "Socket binded" << endl;
}


void sockWrapper :: _listen(size_t number){
    cout << "Listening" << endl;
    listen(this->sock_id, number);
    cout << "exit listen loop"  << endl;
}


void sockWrapper :: _accept(uint16_t port){
    this->sock_id = accept(port, (sockaddr*)&this->addrStruct, (socklen_t*)&this->addrStruct);
    if(sock_id < 0) cerr << "Acception error" << endl;
    else cout << "Accepted" << endl;
}


void sockWrapper :: _recv(char * buffer){
    if(recv(this->sock_id, &buffer, sizeof(buffer), 0) < 0) cerr << "Receiving error";
    cout << "recived" << endl;
}


sockWrapper :: ~sockWrapper(){
    cout << "Socket destructor called" << endl;
    close(this->sock_id);
    cout << " closed connection" << endl;
    memset(&this->addrStruct, 0, sizeof(this->addrStruct));
    cout << "Memset to zero struct" << endl;
}


class cryptoWrapper : public sockWrapper{
    public:
        cryptoWrapper();
        ~cryptoWrapper();
};

#endif //SOCK_WRAPPER_H
