#ifndef UNTITLED4_SERVER_H
#define UNTITLED4_SERVER_H

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
#include "proc.h"
#include <memory.h>

using namespace std;

class Server
{
private:
    uint16_t port = 4430;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

public:
    int sockfd, newsockfd, n, pid;
    char * message;
    Server();
    ~Server();
    void setup();
    double Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2);
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

double Server::Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2){
    string data=to_string(cp)+"/"+to_string(_r1)+"/"+to_string(_r2)+"/"+to_string(hdd_1)+"/"+to_string(hdd_2);
    message = new char[data.length()];
    for (int i = 0; i <= data.length(); i++) {
      message[i] = data[i];
    }
    send(newsockfd, message, sizeof(message),0);
    return 0;
}

void Server::clean(){
    memset(message, 0, sizeof(*message));
    close(sockfd);
    close(newsockfd);
}

#endif //UNTITLED4_SERVER_H
