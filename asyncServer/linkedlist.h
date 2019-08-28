#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>


typedef struct{
    int port, sock;
    socklen_t len;
    struct sockaddr_in clientAddress;
    bool alive;
    node * prev;
    node * next;
}Node;


class LinkedList{
    LinkedList();
    ~LinkedList();
    void add_node();
    Node * node;
    
}


LinkedList :: LinkedList(){
  this->clients[i]->clientAddress.sin_family = AF_INET;
  this->clients[i]->clientAddress.sin_addr.s_addr = AF_ADDR_ANY;
  this->clients[i]->clientAddress.sin_port = htons(this->guest_port);
  this->clietns[i]->len = sizeof((struct sockaddr*)&this->clients->clientAddress);
  this->clients[i]->port = prt;
  this->clients[i]->alive = false;
}


