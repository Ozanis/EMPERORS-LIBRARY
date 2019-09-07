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
#include <cstring>

#define BUFF_SIZE 1024
#define QUEUE_SIZE 5

using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;


class ServerSock{
    public:
        ServerSock(const char * addr, uint16_t port);
        ~ServerSock();
        bool listen_wr();

        int id = 0;
        struct sockaddr_in addrStruct;
};


ServerSock :: ServerSock(const char * addr, uint16_t port){
    this->id = socket(AF_INET, SOCK_STREAM, 0);

    if(this->id < 0) cerr << "Socket creation error" << endl;
    else cout << "Sock created" << endl;

    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = inet_addr(addr);
    this->addrStruct.sin_port = htons(port);
    cout << "Sock initialized " << addr << " " << port << endl;
    
    socklen_t opt = 1;
    switch(setsockopt(this->id, IPPROTO_TCP, SO_REUSEADDR, &opt, sizeof(opt))){
        case(EBADF) : cerr << "Wrong descriptor" << endl; break;
        case(ENOTSOCK) : cerr << "Not socket object" << endl; break;
        case(EFAULT) : cerr << "Forbiden adress" << endl; break;
        case(EINVAL) : cerr << "Wrong optlen" << endl; break;
        case(ENOPROTOOPT) : cerr << "Unknown flag" <<endl; break;
        default: cout << "Socket option set" << endl; break;
    }
    if(bind(this->id, (struct sockaddr *)&this->addrStruct, sizeof(this->addrStruct)) < 0){
        cerr << "Binding error" << endl;
        exit(0);
        }
    else cout << "Socket binded" << endl;
}


bool ServerSock :: listen_wr(){
    cout << "Listening" << endl;
    return listen(this->id, QUEUE_SIZE) >= 0;
}


ServerSock :: ~ServerSock(){
    cout << "Socket destructor called" << endl;
    close(this->id);
    cout << " closed connection" << endl;
    memset(&this->addrStruct, 0, sizeof(this->addrStruct));
    cout << "Memset to zero struct" << endl;
}


class Connector{
    public:
        Connector(int sock, uint16_t port);
        ~Connector();
        bool recv_wr();
        bool is_alive();

        int id = 0;
        char buffer[BUFF_SIZE];
        struct sockaddr_in addrStruct; 
};


Connector :: Connector(int sock, uint16_t port){
    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = htonl(INADDR_ANY);
    this->addrStruct.sin_port = htons(port);

    this->id = accept(sock, (struct sockaddr *)&addrStruct, (socklen_t*)&addrStruct);
    if(this->id < 0) cerr << "Acception  error" << endl;
    else cout << "Sock created: " << this->id << endl;

    char * addr = inet_ntoa(this->addrStruct.sin_addr);
    string s(addr);
    cout << "Sock initialized " << s << " " << port << endl;
}


bool Connector :: recv_wr(){
    return recv(this->id, this->buffer, sizeof(this->buffer), 0) > 0;
}


bool Connector :: is_alive(){
    socklen_t error_code = 0;
    getsockopt(this->id, SOL_SOCKET, SO_ERROR, (socklen_t*)sizeof(error_code), &error_code);
    if(error_code == 0) cout << "Alive" << endl;
    else cout << "Dead" << endl;
    return error_code == 0;
}


Connector :: ~Connector(){
    close(this->id);
}


class ClientSock{
    ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
    ~ClientSock();
    void send_wr(const string & data);
    int id = 0;
    struct sockaddr_in Client;
    struct sockaddr_in Server;
};


ClientSock :: ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port){
   this->id = socket(AF_INET, SOCK_STREAM, 0);
   if(!this->id){
       cerr << "Error client socket creation" << endl;
       exit(0);
       }
    this->Client.sin_family = AF_INET;
    this->Client.sin_addr.s_addr = inet_addr(from_addr);
    this->Client.sin_port = htons(from_port);

    this->Server.sin_family = AF_INET;
    this->Server.sin_addr.s_addr = inet_addr(to_addr);
    this->Server.sin_port = htons(to_port);

    if(connect(this->id, (struct sockaddr *) & this->Server, sizeof(this->Server)) != 0){
        cerr << "Connection error" << endl;
        exit(0);
    }
    else cout << "Connection established" << endl;
}


void ClientSock :: send_wr(const string & data){
    size_t bufsize = data.length();
    char * message = new char[bufsize]{0};
    strcpy(message, data.c_str());
    if(!send(this->id, message, bufsize, 0)) cerr << "Unable to send" << endl;
    delete(message);
}


ClientSock :: ~ClientSock(){
    close(this->id);
}


class cryptoWrapper : public ServerSock{
    public:
        cryptoWrapper();
        ~cryptoWrapper();
};



#endif //SOCK_WRAPPER_H
