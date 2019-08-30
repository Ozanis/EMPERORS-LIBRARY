#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "sockWrapper.h"


using std :: cout;
using std :: endl;


class Node : public sockWrapper{
    public:
        using sockWrapper :: sockWrapper;

        Node(const char * addr, uint16_t port);
        void addNode(uint16_t port);
        Node * pop(Node * node);
        ~Node() = default;

        Node * next = nullptr;
};


void Node :: addNode(const char * addr, uint16_t port){
    this->next = new Node(addr, port);
    cout << "Added node "<< port << endl;
}


Node * Node :: pop(Node * node){
    if (node == nullptr){
        cerr << "Non-existent node" ;
        return nullptr;
        }
    if(this->next != nullptr){
       delete(node);
       return this->next;
    }
    else{
        cout << "Nothing to pop" << endl;
        return node;
    }
}

#endif //LINKED_LIST_H
