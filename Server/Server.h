#include <iostream>
#include <assert.h>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>


using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;


class Server{
public:
    Server(const char * my_addr, uint16_t my_port, const char * guest_addr, uint16_t guest_port);
    ~Server();
    int Up();
    void cast();
    void Recv();
private:
    int sockfd = 0, newsockfd = 0;
    socklen_t len = 0;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
};


Server :: Server(const char * my_addr, uint16_t my_port, const char * guest_addr, uint16_t guest_port) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd < 0) cerr << "Error of socket opening-1";
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(my_addr);
    this->serverAddress.sin_port = htons(my_port);

    this->clientAddress.sin_family = AF_INET;
    this->clientAddress.sin_addr.s_addr = inet_addr(guest_addr);
    this->clientAddress.sin_port = htons(guest_port);
    this->len = sizeof((struct sockaddr*)&this->clientAddress);
}


Server :: ~Server(){
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    memset(&this->clientAddress, 0, sizeof(this->serverAddress));
    shutdown(this->sockfd, 2);
    shutdown(this->newsockfd, 2);
    close(this->sockfd);
    close(this->newsockfd);
    delete(&serverAddress);
    delete(&clientAddress);
}


int Server :: Up(){
   socklen_t enable = 1;
   if(setsockopt(this->sockfd, IPPROTO_TCP, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
       return bind(this->sockfd,(struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));
   else return -1;
}


void Server :: cast(){
    listen(this->sockfd, 5);
}


void Server :: Recv(){
    this->newsockfd = accept(this->sockfd, (sockaddr*)&this->clientAddress, (socklen_t*)&this->len);
    if(this->newsockfd < 0){
        cerr << "Acception error";
        return;
    }
    char message[512]{0};
    recv(this->newsockfd, &message, sizeof(message), 0);
    string telemetry(message);
    cout << telemetry << endl;
    memset(message, 0, sizeof(message));
}
