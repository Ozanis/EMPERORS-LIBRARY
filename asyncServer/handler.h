#ifndef HANDLER_H
#define HANDLER_H

#include "linkedlist.h"


using std :: cerr;         
using std :: cout;         
using std :: endl;         


class Handler{
    public:
        Handler(const char * addr, uint16_t port, size_t max_connectors);
        ~Handler();
        void update();
        void add_connection();
        bool is_alive(int id);

        size_t connectors = 0, max_num = 0;
        uint16_t port_handler = 0;
        struct pollfd fds;
        Node * linkedlist;
};


Handler :: Handler(const char * addr, uint16_t port, size_t max_connectors){
    this->linkedlist = new Node(addr, port);
    this->port_handler = port;
    this->max_num = max_connectors;
}


bool Handler :: is_alive(int id){
    this->fds.fd = id;
    this->fds.events = POLLIN;
    int ret = poll(&this->fds, sizeof(this->fds), 10);
    this->fds.revents = 0;
    return ret & POLLIN;
}


void Handler :: update(){
    size_t cons = 0;
    Node * connection = this->linkedlist;
    while(connection != nullptr){
        if(is_alive(connection->sock_id)) ++cons;
        else{
            Node * temp = connection->pop(connection);
            if(temp != nullptr){
                delete(temp);
                connection->next = temp;
            }
        }
        connection = connection->next;
    }
    this->connectors = cons;
}


void Handler :: add_connection(){
    if(this->connectors >= this->max_num) cerr << "Connectors number overflow";
    else{
        addNode(INADDR_ANY, this->port_handler++);
        ++this->connectors;
    }
}


Handler :: ~Handler(){
    if(this->linkedlist != nullptr){
        Node * next = this->linkedlist;
        while(next != nullptr){
            Node * cur = next;
            next = cur->next;
            delete(cur);
        }
    }
}

#endif //HANDLER_H
