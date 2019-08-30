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
       using Handler :: Handler;
       Server(const char * my_addr, uint16_t my_port, size_t max_connectors);
       ~Server() = default;
       void recive(Node * connection);
       void display(Node * connection);
       void cast();

       sockWrapper * server_socket = nullptr;
       char *  buffer;
};


Server :: Server(const char * my_addr, uint16_t my_port, size_t max_connectors) {
    this->server_socket = new sockWrapper(my_addr, my_port);
    this->server_socket->_bind();
    this->max_num = max_connectors;
    this->buffer = new char[sizeof(char)*BUFFSIZE];
}


void Server :: cast(){
    Node * connection = this->linkedlist;
    while(connection != nullptr){
        if(is_alive(connection->sock_id)) recive(connection);
        connection = connection->next;
   }
}


void Server :: display(Node * connection){
    string telemetry(this->buffer);
    char * host = nullptr;
    socklen_t len = 16;
    if(inet_ntop(AF_INET, &connection->addrStruct.sin_addr.s_addr, host, len) == nullptr){
        string str(host);
        cout << "From " << str << " :";    }
    else cout  << "[Unable to decode connector address] ";
    cout << telemetry << endl;
    delete(&telemetry);
}


void Server :: recive(Node * connection){
    this->server_socket->_recv(this->buffer);
    display(connection);
}


class AsyncServer : public Server{
    public:
        using Server :: Server;
        explicit AsyncServer(const char * my_addr, uint16_t my_port, size_t max_connectors);
        ~AsyncServer() = default;
        void asyncCast();
};


void AsyncServer :: asyncCast(){
    cast();
    this->server_socket->_listen(this->max_num);
    add_connection();
}


#endif //ASYNC_H
