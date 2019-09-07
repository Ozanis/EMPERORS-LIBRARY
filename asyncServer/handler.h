#ifndef HANDLER_H
#define HANDLER_H

#include "linkedlist.h"


using std :: cerr;         
using std :: cout;         
using std :: endl;         


class Handler{
    public:
        explicit Handler(uint16_t port) : port_handler(port){};
        ~Handler();
        void update();
        void add_connection(int id);

        size_t connectors = 0;
        uint16_t port_handler = 0;
        Node * linkedlist;
};


void Handler :: update(){
    size_t cons = 0;
    Node * connection = this->linkedlist;
    while(connection != nullptr){
        if(connection->is_alive()) ++cons;
        connection = connection->next;
        }
    this->connectors = cons;
}


void Handler :: add_connection(int id){
    cout << "Adding new connecor to linkedlist" << endl;
    if(!this->connectors){
        this->linkedlist = new Node(id, this->port_handler);
        cout << "First connection" << endl;
        ++this->connectors;
    }
    else if(this->connectors >= QUEUE_SIZE) cerr << "Connectors number overflow" << endl;
    else{
        this->linkedlist->add(id, this->port_handler++);
        ++this->connectors;
        }
    cout << "Current num of connectors" << this->connectors << endl;
}


Handler :: ~Handler(){
    cout << "Destruction of handler" << endl;
    /*
    if(this->linkedlist != nullptr){
        Node * prev = this->linkedlist;
        while(prev != nullptr){
            Node * cur = prev;
            prev = cur->prev;
            delete(cur);
        }
    }*/
}

#endif //HANDLER_H
