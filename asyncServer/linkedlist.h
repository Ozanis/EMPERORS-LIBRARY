#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>


using std :: cerr;
using std :: cout;
using std :: endl;


class Node{
    public:
        Node();
        ~Node();
        set(uint16_t my_port);

        uint16_t port = 0;
        int sock = 0;
        bool alive = false;
        socklen_t len = 0;
        Node * next = nullptr;
        struct sockaddr_in client;
};


Node :: Node(){
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sock < 0) cerr << "Socket creation error";
    this->len = sizeof((struct sockaddr*)&this->client);
}


Node :: set(uint16_t my_port){
     this->port = my_port;
     this->client.sin_family = AF_INET;
     this->client.sin_addr.s_addr = INADDR_ANY;
     this->client.sin_port = htons(my_port);
}


Node :: ~Node(){
    if(this->next != nullptr) this->next = next;
    shutdown(this->sock, 2);
    close(this->sock);
    memset(&this->client, 0, sizeof(this->client));
//    delete(&this->next);
}

#endif //LINKED_LIST_H
