#ifndef ASYNC_H
#define ASYNC_H

#include <thread>
#include <mutex>
#include "handler.h"


using std :: cout;
using std :: endl;
using std :: cerr;
using std :: string;
using std :: thread;
using std :: mutex;


class Server : public Handler, public  ServerSock{
    public:
       explicit Server(const char * addr, uint16_t port) : Handler(port), ServerSock(addr, port){};
       ~Server() = default;
       void recive(Node * connection);
       void display(Node * connection);
       void cast();
};


void Server :: recive(Node * connection){
    cout << "Start to recive" << endl;
//    if(connection->is_alive()){
      if (connection->recv_wr()) display(connection);
    else{
        cerr << "Dead connection "
//        << getpeername(connection->id , (struct sockaddr*)&connection->addrStruct, (socklen_t*)sizeof(connection->addrStruct))
        << endl;
        --this->connectors;
        delete connection;
    }
}


void Server :: display(Node * connection){
    cout << "displaying" << endl;
    char * host = inet_ntoa(connection->addrStruct.sin_addr);
    string str(host), telemetry(connection->buffer);
    cout << "---> " << str << " : " << telemetry << endl;
//    memset(&connection->buffer, 0, BUFF_SIZE);
//    delete(telemetry);
//    delete(str);
//    delete(host);
}


void Server :: cast(){
    if(this->connectors == 0) cout << "No clietns" << endl;
    else{
     cout << "Cast loop iteration" << endl;
     Node * connection = this->linkedlist;
     while(connection != nullptr){
        Node * temp = connection;
        connection = temp->next;
        cout << "Check connection" << endl;
        recive(temp);
        }
     cout << "Switching to the next connection" << endl;
     }
}


class AsyncServer : public Server{
    public:
        explicit AsyncServer(const char * addr, uint16_t port) : Server(addr, port){};
        ~AsyncServer() = default;
        void add_client();
        void asyncCast();
//        void asyncListen();
};

/*
void AsyncServer :: asyncListen(){
    if (listen_wr()){
        cout << "Adding connection" << endl;
        add_connection(this->id);
    }
}
*/

void AsyncServer :: add_client(){
    add_connection(this->id);
}


void AsyncServer :: asyncCast(){
    while(1) cast();
}


#endif //ASYNC_H
