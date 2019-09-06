#ifndef SOCK_WRAPPER_H
#define SOCK_WRAPPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include "unistd.h"
#include <iostream>
#include "cstring"


using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;


class SockWrapper{
   public:
        virtual bool _listen() = 0;
        virtual bool _recv() = 0;
        virtual bool _accept() = 0;
        virtual bool _is_alive() = 0;
        virtual void _send() = 0;
}


class ServerSock : SockWrapper{
    public:
        ServerSock(const char * addr, uint16_t port, size_t size);
        ~ServerSock();
        bool _listen();

        int server_sock = 0;
        size_t queue_size = 0;
        struct sockaddr_in addrStruct;
};


ServerSock :: ServerSock(const char * addr, uint16_t port, size_t size){
    this->server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(this->server_sock < 0) cerr << "Socket creation error" << endl;
    else cout << "Sock created" << endl;

    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = inet_addr(addr);
    this->addrStruct.sin_port = htons(port);
    cout << "Sock initialized " << addr << " " << port << endl;
    
    socklen_t opt = 1;
    switch(setsockopt(this->server_sock, IPPROTO_TCP, SO_REUSEADDR, &opt, sizeof(opt))){
        case(EBADF) : cerr << "Wrong descriptor" << endl; break;
        case(ENOTSOCK) : cerr << "Not socket object" << endl; break;
        case(EFAULT) : cerr << "Forbiden adress" << endl; break;
        case(EINVAL) : cerr << "Wrong optlen" << endl; break;
        case(ENOPROTOOPT) : cerr << "Unknown flag" <<endl; break;
        default: cout << "Socket option set" << endl; break;
    }

    bind(this->sock_id, (struct sockaddr *)&this->addrStruct, sizeof(this->addrStruct));
    if(this->sock_id < 0){
        cerr << "Binding error" << endl;
        exit(0);
    }
    else cout << "Socket binded" << endl;
}


bool ServerSock :: _listen(){
    cout << "Listening" << endl;
    return listen(this->sock_id, this->queue_size) > 0;
}


ServerSock :: ~ServerSock(){
    cout << "Socket destructor called" << endl;
    close(this->sock_id);
    cout << " closed connection" << endl;
    memset(&this->addrStruct, 0, sizeof(this->addrStruct));
    cout << "Memset to zero struct" << endl;
}


class Connector : public SockWrapper{
    public:
        Connector(int id, uint16_t port);
        ~Connector();
        bool _recv(char * buffer);
        bool _is_alive();

        int sock_id = 0;
        struct sockaddr_in addrStruct;
}


Connector :: Connector(int id, uint16_t port){
    this->sock_id = id;
    this->sock_id = accept(id, (struct sockaddr *)&addrStruct, (socklen_t*)&addrStruct);
        //socket(AF_INET, SOCK_STREAM, 0);

    if(this->sock_id < 0) cerr << "Acception  error" << endl;
    else cout << "Sock created " << endl;

    this->addrStruct.sin_family = AF_INET;
//    this->addrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
    this->addrStruct.sin_port = htons(port);
    char * addr = inet_ntoa(this->addrStruct.sin_addr);
    string s(addr);
    cout << "Sock initialized " << s << " " << port << endl;
}


bool Connector :: _recv(char * buffer){
    return recv(this->sock_id, &buffer, sizeof(buffer), 0) < 0;
}


bool Connector :: _is_alive(){
    socklen_t error_code = 0;
    getsockopt(this->sock_id, SOL_SOCKET, SO_ERROR, (socklen_t*)sizeof(error_code), &error_code);
    if(error_code == 0) cout << "Alive" << endl;
    else cout << "Dead" << endl;
    return error_code == 0;
}


Connector :; ~Connector(){
    close(this->sock_id);
}


class ClientSock{
    ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
    ~ClientSock();
    void _send(const char * data);

    int sock_id = 0;
    struct sockaddr_in Client;
    struct sockaddr_in Server;
}


ClientSock :: ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port{
   this->sock_id = socket(AF_INET, SOCK_STREAM, 0);
   if(!this->sock_id){
       cerr << "Error client socket creation" << endl;
       exit(0);
       }
    this->Client.sin_family = AF_INET;
    this->Client.sin_addr.s_addr = inet_addr(from_addr);
    this->Client.sin_port = htons(from_port);

    this->Server.sin_family = AF_INET;
    this->Server.sin_addr.s_addr = inet_addr(to_addr);
    this->Server.sin_port = htons(to_port);

    if(connect(this->sock_id, (struct sockaddr *) & this->Server, sizeof(this->Server)) != 0){
        cerr << "Connection error" << endl;
        exit(0);
    }
    else count << "Connection established" << endl;
}


void ClientSock :: _send(const char & data){
    size_t bufsize = data.lenght();
    char * message = new char[bufsize]{0};
    strcpy(message, data.c_str());
    if(!send(this->sock_id, message, buffsize, 0)) cerrr << "Unable to send" << endl;
    delete(message);
}


ClientSock :: ~ClientSock(){
    close(this->sock_id);
}



class cryptoWrapper : public ServerSock{
    public:
        cryptoWrapper();
        ~cryptoWrapper();
};



#endif //SOCK_WRAPPER_H
