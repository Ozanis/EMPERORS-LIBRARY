#include <iostream>
#include <thread>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024


using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;
using std :: exception;


class Server : public Handler{
public:
    Server(const char * my_addr, uint16_t my_port, size_t connectors);
    ~Server();
    bool Up();
    void cast();
    void ether();
   
private:
    int sockfd = 0;
    size_t num_of_clients = 0, cur_num = 0;;
    struct sockaddr_in serverAddress;
    char buffer[BUFSIZE];
    void recv();
    void add_connect();
};


Server :: Server(const char * my_addr, uint16_t my_port, size_t connectors) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd < 0) cerr << "Error of socket opening-1";
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(my_addr);
    this->serverAddress.sin_port = htons(my_port);
    this->num_of_clients = connectors;
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
