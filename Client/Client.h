#include <iostream>
#include <assert.h>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

using std::cerr;
using std::string;
using std::to_string;

class Client{
private:
    uint16_t port=44300;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

public:
    int sockfd, newsockfd;
    Client();
    ~Client();
    void set_up();
    void conn();
    void Send(string data);
    socklen_t len;
};

Client::Client(){
    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(!this->sockfd) cerr << "Error of socket opening-1";

    this->serverAddress.sin_family=AF_INET;
    this->serverAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->serverAddress.sin_port=htons(this->port);

    this->clientAddress.sin_family=AF_INET;
    this->clientAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->clientAddress.sin_port=htons(this->port);
    this->len = sizeof((struct sockaddr*)&this->clientAddress);
}

Client::~Client(){
    memset(&this->serverAddress,0,sizeof(this->serverAddress));
    close(this->sockfd);
    close(this->newsockfd);
}

void Client::set_up(){
    if((bind(this->sockfd,(struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress)))<0) {
        cerr << "Error of binding";
        socklen_t sndbuf = 512;
        setsockopt(this->sockfd, IPPROTO_TCP, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    }
}

void Client::conn(){
    listen(this->sockfd, 1);
    this->newsockfd=accept(this->sockfd,(sockaddr*)&this->clientAddress, (socklen_t*)&this->len);
    if(this->newsockfd<0) cerr << "Acception error";
    if(!gethostbyname("127.0.0.1")) cerr << "Host is unreachable";
}

void Client::Send(string data){
    if(!gethostbyname("127.0.0.1")) {
        cerr << "Host is unreachable";
        return;
    }
    auto message = new char[data.length()];
    strcpy(message, data.c_str());
    send(this->newsockfd, message, sizeof(message), 0);
    memset(message, 0, sizeof(*message));
}
