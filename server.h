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

class Server
{
private:
    uint16_t port = 44300;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

public:
    int sockfd, newsockfd;
    char * message;
    Server();
    ~Server();
    bool set_up();
    bool Send(string data);
    int len;
};

Server::Server(){
    this->sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(!this->sockfd) cerr << "Error of socket opening-1";
    this->serverAddress.sin_family=AF_INET;
    this->serverAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->serverAddress.sin_port=htons(this->port);

    this->newsockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(!this->newsockfd) cerr << "Error of socket opening-2";
    this->clientAddress.sin_family=AF_INET;
    this->clientAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->clientAddress.sin_port=htons(this->port);
    this->len = sizeof(clientAddress);
}

Server::~Server(){
    memset(message, 0, sizeof(*message));
    memset(&serverAddress,0,sizeof(serverAddress));
    close(sockfd);
}

bool Server::set_up(){
    if((bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress)))<0) {
        cerr << "Error of binding";
        return false;
    } listen(sockfd, 1);
    newsockfd=accept(sockfd,(sockaddr*)&clientAddress, (socklen_t*)&len);
    if(newsockfd<0) {
        cerr << "Acception error";
        return false;
    };
    return true;
}

bool Server::Send(string data){
    if(!gethostbyname("127.0.0.1")){
        cerr<<"Host is unreachable";
        return false;
    } message = new char[data.length()];
    strcpy(message, data.c_str());
    return send(newsockfd, message, sizeof(message), 0)<0;
}
