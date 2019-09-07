#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


using std::cerr;
using std::string;
using std::to_string;


class Client{
public:
    Client(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
    ~Client();
    bool Up();
    void Send(const string & data);
    char * message = nullptr;

    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int sockfd = 0;
};


Client :: Client(const char * from_addr, uint16_t from_port, const char * to_addr,  uint16_t to_port){
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->sockfd) cerr << "Error of socket opening-1";

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(to_addr);
    this->serverAddress.sin_port = htons(to_port);

    this->clientAddress.sin_family = AF_INET;
    this->clientAddress.sin_addr.s_addr = inet_addr(from_addr);
    this->clientAddress.sin_port = htons(from_port);
}

Client :: ~Client(){
    close(this->sockfd);
}


bool Client :: Up(){
    return connect(this->sockfd, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress)) == 0;
}


void Client :: Send(const string & data){
    size_t bufsize = data.length();
    this->message = new char[bufsize];
    strcpy(message, data.c_str());
    if(!send(this->sockfd, message, bufsize, 0)) cerr << "unable to send";
    memset(&message, bufsize, 0);
}
