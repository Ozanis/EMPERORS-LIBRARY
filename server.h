#include <iostream>
#include <assert.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>


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
    void setup();
    bool Send(string data);
    void clean();
};

Server::Server(){}

Server::~Server(){}

void Server::setup() {
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    serverAddress.sin_port=htons(port);
    bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(sockfd, 1);
}

bool Server::Send(string data){
    message = new char[data.length()];
    for (int i = 0; i <= data.length(); i++) {
      message[i] = data[i];
    }
    return send(newsockfd, message, sizeof(message), 0) < 0;
}

void Server::clean(){
    memset(message, 0, sizeof(*message));
    close(sockfd);
    close(newsockfd);
}