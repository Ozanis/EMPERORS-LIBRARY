#ifndef HANDLER_H
#define HANDLER_H

#include "linkedlist.h"

using std :: cerr;         
using std :: cout;         
using std :: endl;         


class Handler{
    public:
        Handler(uint16_t my_port, size_t max_connectors);
        ~Handler();
        void update();
        void add_connection();
        bool is_alive(Node * connection);
        void handle_connections();
        size_t connectors = 0, max_num = 0;
        uint16_t port = 0;
        Node * llist;
        Node * last_node;
}


Handler :: Handler(uint16_t my_port, size_t max_connectors){
    this->llist = new Node(my_port);
    this->port = my_port;
    this->max_num = max_connectors;
}


bool Handler :: is_alive(Node * connection){
    if(this->connectors){
        size_t set = 0;
        if(!FD_ISSET(connection->sock, handle_read)){
            try{
                 delete(connection);
            }
            catch(Node * connection){
                throw std::exception("Socket have been closed already");
                connection->alive = false;
                connection = nullptr;
            }
         }
        return false;
    }
    return true;
}


void Handler :: update(){
    size_t cons = 0;
    Node * connection = this->llist;
    do {
        if(is_alive(connection)) ++cons;
        connection = connection->next;
    } while(connection != nullptr);
    this->connectors = cons;
    this->last_node = connection;
}


void Handler :: add_connection(){
    if(this->connectors >= this->max_num){
       cerr << "Connectors num overflow";
       return;
    }
    this->last_node = this->last_node->next;
    this->last_node = Node();
    this->last_node.set(this->port++);
    int port_id = accept(this->last_node->sock, (sockaddr*)&this->last_node->client, (socklen_t*)&this->last_node->len);
        if(port_id < 0){
        cerr << "Acception error";
        return;
    }
    this->last->node->set(port_id);
    ++this->connectors;
}


void Handler :: handle_connections(){
    listen(this->server_socket, this->max_num);
    add_connection();
}


Handler :: ~Handler(){
    Node * last = this->llist;
    Node * next = this->llist;
    while(next != nullptr){
        next = last->next;
        delete(&last);
    }
}

#endif //HANDLER_H
