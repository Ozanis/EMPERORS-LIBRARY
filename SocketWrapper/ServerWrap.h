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
#include "fcntl.h"


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
//  cout << fcntl(this->id, F_SETFL, O_NONBLOCK) << endl;
    this->addrStruct.sin_family = AF_INET;
    this->addrStruct.sin_addr.s_addr = inet_addr(addr);
    this->addrStruct.sin_port = htons(port);
    cout << "Server sock initialized " << addr << " " << port << endl;
    
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

    this->id = accept(sock, (struct sockaddr *)&this->addrStruct, (socklen_t*)&this->addrStruct);
//  cout << fcntl(this->id, F_SETFL, O_NONBLOCK) << endl;
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
    struct pollfd fd;
    fd.fd = this->id;
    fd.events = POLLIN;
    cout << fd.events << endl;
    switch(poll(&fd, 0, 1000)){
        case EBADF:{ cerr << EBADF << endl; return false;}
        case ENOMEM:{ cerr << ENOMEM << endl; return false;}
        case EFAULT:{ cerr << EFAULT << endl; return false;}
        case EINTR:{ cerr << EBADF << endl; return false;}
        default: break;
    }
    cout << fd.revents << endl;
    return fd.revents & POLLIN; // POLLHUP; //POLLRDNORM; //POLLMSG;
    /*
    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(this->id, &rfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    retval = select(this->id, &rfds, NULL, NULL, &tv);
    cout << retval <<  FD_ISSET(this->id, &rfds) << endl;
    return retval;
    */
    /*
    socklen_t error_code = 0;
    getsockopt(this->id, SOL_SOCKET, SO_ERROR, (socklen_t*)sizeof(error_code), &error_code);
    if(error_code == 0) cout << "Alive" << endl;
    else cout << "Dead" << endl;
    return error_code == 0;
    */
}


Connector :: ~Connector(){
    close(this->id);
}


#endif //SOCK_WRAPPER_H
