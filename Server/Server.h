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
#include "dcmprs.h"

using std :: cerr;
using std :: cout;
using std :: endl;


class Server{
private:
    uint16_t port = 44301;
    struct sockaddr_in serverAddress;

public:
    int sockfd;
    Server();
    ~Server();
    void set_up();
    bool access();
    int off();
    void Recv();
};

Server::Server(){
    this->sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(!this->sockfd) cerr << "Error of socket opening-1";
    this->serverAddress.sin_family=AF_INET;
    this->serverAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->serverAddress.sin_port=htons(this->port);
    }

Server::~Server(){
    memset(&this->serverAddress,0,sizeof(this->serverAddress));
    close(this->sockfd);
}

void Server::set_up(){
    socklen_t enable = 1;
    setsockopt(this->sockfd, IPPROTO_TCP, SO_REUSEADDR, &enable, sizeof(enable));
    connect(this->sockfd, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));
}

bool Server::access(){
    return gethostbyname("127.0.0.1");
}

void Server::Recv(){
    char message[1024];
    recv(this->sockfd, &message, sizeof(message), 0);
    string telemetry(message);
    cout << telemetry << endl;
    cout << message << endl;
    Gzip msg;
    string val = msg.decompress(telemetry);
    cout << val << endl;
    bzero(message, sizeof(*message));
}


