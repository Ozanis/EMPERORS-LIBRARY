#include <iostream>
#include <assert.h>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

using std::cerr;
using std::string;
using std::to_string;

class Server
{
private:
    uint16_t port = 4430;
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
    if(!this->sockfd) cerr << "Error of socket opening";
    this->serverAddress.sin_family=AF_INET;
    this->serverAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    this->serverAddress.sin_port=htons(this->port);

    this->newsockfd=socket(AF_INET, SOCK_STREAM, 0);
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

    if(accept(sockfd,(sockaddr*)&clientAddress, (socklen_t*)&len)<0) {
        cerr << "Acception error";
        return false;
    };

    return true;
}

bool Server::Send(string data){
    message = new char[data.length()];
    for (int i = 0; i <= data.length(); i++) {
      message[i] = data[i];
    }
    return send(sockfd, message, sizeof(message), 0) < 0;
}
