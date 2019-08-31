#ifndef ASYNC_H
#define ASYNC_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include "string.h"
#include "handler.h"

#define BUFFSIZE 1024


using std :: cout;
using std :: endl;
using std :: cerr;
using std :: string;


class Server : public Handler {
    public:
       Server(const char * addr, uint16_t port, size_t max_connectors);
       ~Server() = default;
       void recive(Node * connection);
       void display(Node * connection);
       void cast();

       sockWrapper * server_socket; //(const char * addr, uint16_t port);
       char buffer[BUFFSIZE];
};


Server :: Server(const char * addr, uint16_t port, size_t max_connectors) : Handler(port, max_connectors){
    cout << "Server creation" << endl;
    this->server_socket = new sockWrapper(addr, port);
    this->server_socket->_bind();
    socklen_t opt = 1;
    switch(setsockopt(this->server_socket->sock_id, IPPROTO_TCP, SO_REUSEADDR, &opt, sizeof(opt))){
        case(EBADF) : cerr << "Wrong descriptor" << endl; break;
        case(ENOTSOCK) : cerr << "Not socket object" << endl; break;
        case(EFAULT) : cerr << "Forbiden adress" << endl; break;
        case(EINVAL) : cerr << "Wrong optlen" << endl; break;
        case(ENOPROTOOPT) : cerr << "Unknown flag" <<endl; break;
        default: cout << "Socket option set" << endl;
    }
//    this->buffer = new char[sizeof(char)*BUFFSIZE];
}


void Server :: cast(){
    cout << "Cast loop iteration" << endl;
    Node * connection = this->linkedlist;
    while(connection != nullptr){
        Node * temp = connection;
        connection = temp->next;
        cout << "Check connection" << endl;
        if(is_alive(temp->sock_id)){
            cout << "Alive!" << endl;
            recive(connection);
        }
        cout << "Switching to the next connection" << endl;
    }
}


void Server :: display(Node * connection){
    cout << "displaying" << endl;
    char * host = inet_ntoa(connection->addrStruct.sin_addr);
    std :: string str(host);
    string telemetry(this->buffer);
    cout << "From " << str << " : " << telemetry << endl;
    delete(&telemetry);
    delete(&str);
    delete(host);
}


void Server :: recive(Node * connection){
    cout << "Start to recive" << endl;
    this->server_socket->_recv(this->buffer);
    display(connection);
}


class AsyncServer : public Server{
    public:
        explicit AsyncServer(const char * addr, uint16_t port, size_t max_connectors) : Server(addr, port, max_connectors){};
        ~AsyncServer() = default;
        void asyncCast();
};


void AsyncServer :: asyncCast(){
    cast();
    this->server_socket->_listen(this->max_num);
    add_connection();
}


#endif //ASYNC_H
