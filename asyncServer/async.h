#ifndef ASYNC_H
#define ASYNC_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include "string.h"
#include "handler.h"


#define BUFSIZE 1024


using std :: string;


class Server : public Handler {
public:
    using Handler :: Handler;
    Server(const char * my_addr, uint16_t my_port, size_t max_connectors);
    ~Server();
    void cast()

private:
    int server_socket = 0;
    char buffer[BUFSIZE];
    uint16_t server_port = 0;
    socklen_t sock_opt = 1;
    struct sockaddr_in serverAddress;
    void recive(Node * connection);
};


Server :: Server(const char * my_addr, uint16_t my_port, size_t max_connectors) {
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->server_socket < 0) cerr << "Error of socket opening-1";
    if(setsockopt(this->server_socket, IPPROTO_TCP, SO_REUSEADDR, (void*)sock_opt, sizeof(socklen_t)) < 0 ) cerr <<  "Error of setting option";
    this->server_port = my_port;
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(my_port);
    if(bind(this->server_socket, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress)) < 0) cerr << "Binding error";
}


Server :: ~Server(){
    shutdown(this->server_socket, 2);
    close(this->server_socket);
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    delete(&serverAddress);
}


void Server :: cast(){
    Node * connection = this->llist;
    while(connection != nullptr){
        if(is_alive(connection)) recive(connection);
        connection = connection->next;
    }
    listen(this->server_socket, this->max_num);
    add_connection();
}


void Server :: recive(Node * connection){
    if(recv(connection->sock, &this->buffer, sizeof(this->buffer), 0) < 0) {
        cerr << "Receiving error";
        if(is_alive(connection)) cerr << "But connection is alive";
        else cerr << "Because connection is dead";
    }
    string telemetry(this->buffer);
    cout << "From " << inet_ntoa(connection->client.sin_addr.s_addr) << " :" << telemetry << endl;
    memset(this->buffer, 0, BUFSIZE);
}

#endif //ASYNC_H
