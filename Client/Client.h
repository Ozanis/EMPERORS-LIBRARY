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
public:
    Client(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
    ~Client();
    int Up();
    void Send(const string & data);

private:
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int sockfd = 0;
//    socklen_t len;
};


Client :: Client(const char * from_addr, uint16_t from_port, const char * to_addr,  uint16_t to_port){
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->sockfd) cerr << "Error of socket opening-1";

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(from_addr);
    this->serverAddress.sin_port = htons(from_port);

    this->clientAddress.sin_family = AF_INET;
    this->clientAddress.sin_addr.s_addr = inet_addr(to_addr);
    this->clientAddress.sin_port = htons(to_port);
//    this->len = sizeof((struct sockaddr*)&this->clientAddress);
}

Client :: ~Client(){
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    close(this->sockfd);
}

int Client :: Up(){
    socklen_t sndbuf = 512;
    if(setsockopt(this->sockfd, IPPROTO_TCP, SO_SNDBUF, &sndbuf, sizeof(sndbuf)) < 0)
        return connect(this->sockfd, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));
    else return -1;
}

using std :: cout;
using std :: endl;

void Client :: Send(const string & data){
    uint bufsize = data.length();
    auto message = new char[bufsize];
    strcpy(message, data.c_str());
    cout << message << endl;
    if(!send(this->sockfd, message, bufsize, 0)){
        cerr << "unable to send";
        return;
    }
    memset(message, 0, sizeof(*message));
}