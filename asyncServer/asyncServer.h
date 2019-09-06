#ifndef ASYNC_H
#define ASYNC_H

#include <thread>
#include "handler.h"

#define BUFFSIZE 1024


using std :: cout;
using std :: endl;
using std :: cerr;
using std :: string;


class Server : public Handler, ServerSock{
    public:
       explicit Server(const char * addr, uint16_t port, size_t max_connectors) : Handler(max_connectors), ServerSock(addr, port, max_connectors);
       ~Server() = default;
       void recive(Node * connection);
       void display(Node * connection);
       void cast();

       char buffer[BUFFSIZE]{0};
};


void Server :: cast(){
    if(this->connectors == 0) cout << "No clietns" << endl;
    else{
     cout << "Cast loop iteration" << endl;
     Node * connection = this->linkedlist;
     while(connection != nullptr){
        Node * temp = connection;
        connection = temp->next;
        cout << "Check connection" << endl;
        connection->_recive(this->buffer);
        }
    cout << "Switching to the next connection" << endl;
     }
}


void Server :: display(Node * connection){
    cout << "displaying" << endl;
    char * host = inet_ntoa(connection->addrStruct.sin_addr);
    string str(host), telemetry(this->buffer);
    cout << "From " << str << " : " << telemetry << endl;
    delete(&telemetry);
    delete(&str);
    delete(host);
}


void Server :: recive(Node * connection){
    cout << "Start to recive" << endl;
    if (connection->_recv(this->buffer)) display(connection);
    else{
        cerr << "Dead connection" << endl;
        delete connection;
    }
}


class AsyncServer : public Server{
    public:
        explicit AsyncServer(const char * addr, uint16_t port, size_t max_connectors) : Server(addr, port, max_connectors){};
        ~AsyncServer() = default;
        void asyncCast();
};


void AsyncServer :: asyncCast(){
    cast();
    if (this->server_socket->_listen()) add_connection();
}


#endif //ASYNC_H
