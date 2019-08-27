#include <iostream>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>


#define BUFSIZE 1024


using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;
using std :: exception;

typedef struct {
    int port;
    int sock;
    socklen_t len;
    struct sockaddr_in clientAddress;
    bool alive;
}Clients;


class Server{
public:
    Server(const char * my_addr, uint16_t my_port, size_t connectors);
    ~Server();
    bool Up();
    void cast();
    void ether();
   
private:
    int sockfd = 0;
    size_t num_of_clients = 0; 
    Clients * clients = nullptr;
    struct sockaddr_in serverAddress;
    char buffer[BUFSIZE];
    void recv();
    void add_connect();
    size_t is_alive();
};


Server :: Server(const char * my_addr, uint16_t my_port, size_t connectors) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd < 0) cerr << "Error of socket opening-1";
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(my_addr);
    this->serverAddress.sin_port = htons(my_port);
    this->num_of_clients = connectors;
    this->clients = new clients[sizeof(clients)];
    for(size_t i = 0, uint16_t prt = ++my_port; i < connectors; i++, prt++){
         this->clients[i]->clientAddress.sin_family = AF_INET;
         this->clients[i]->clientAddress.sin_addr.s_addr = AF_ADDR_ANY;
         this->clients[i]->clientAddress.sin_port = htons(this->guest_port);
         this->clietns[i]->len = sizeof((struct sockaddr*)&this->clients->clientAddress);
         this->clients[i]->port = prt;
         this->clients[i]->alive = false;
    }
}


Server :: ~Server(){
    shutdown(this->sockfd, 2);
    close(this->sockfd);
    for(size_t i = 0; i < this->num_of_clients; i++){
        shutdown(this->clients[i]->sock, 2);
        close(this->clients[i]->sock);
        memset(&this->clients->clientAddress[i], 0, sizeof(this->clients->clientAddress[i]));
        delete(&clientAddress[i]);
       }
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    delete(&serverAddress);
}


bool Server :: Up(){
   if(setsockopt(this->sockfd, IPPROTO_TCP, SO_REUSEADDR, &enable, sizeof(socklen_t)) < 0) return false;
   return bind(this->sockfd,(struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));
}


void Server :: cast(){
    listen(this->sockfd, this->num_of_clients);
}


void Server add_connection(){
    if(this->cur_num >= this->num_of_clients)
       Cerr << "Connectors num overflow";
       return;
    int newsockfd = accept(this->sockfd, (sockaddr*)&this->clients->clientAddress, (socklen_t*)&clients->this->len);
    if(this->newsockfd < 0){
        cerr << "Acception error";
        return;
    }
    for(size_t i = 0; i < this->num_of_client; i++){
        if(!this->clients[i]->alive){
            this->clients[i]->alive = true;
            this->clients[i]->sock = newsockfd;
        }
    }
}


void Server :: is_alive(){
   for(size_t i = 0; i < this->num_of_clients; i++){
       if(FD_ISSET(this->guest_port[i], handle_read)) ++set;
       else{
           ++not_set;
           this->clients[i]->alive = false;
           try{
                shutdown(this->clients[i]->sock, 2);
                close(this->clients[i]->sock);
                }
            throw exception("Socket have closed already");
        }
    }
}


void Server :: recv(struct sockaddr_in * client, int sock){
    recv(sock, &this->buffer, sizeof(this->buffer), 0);
    string telemetry(this->buffer);
    cout << "From " << inet_ntoa(client->sin_addr->S_addr) << " :" << telemetry << endl;
    memset(this->buffer, 0, BUFSIZE);
}


void Server :: ether(){
    for(size_t i = 0; i < this->num_of_clients; i++){
        if(this->clients[i]->alive){
            recv(&this->clients[i]->clientAdress, clients[i]->sock);
        }
    }
}
