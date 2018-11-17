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

using namespace std;

class Server
{
public:
    int sockfd, newsockfd, n, pid;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    char * message;
    Server();
    ~Server();
    void setup(uint16_t port);
    double Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2);
    void clean();
};

Server::Server(){}

Server::~Server(){}

void Server::setup(uint16_t port) {
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port=htons(port);
    bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(sockfd, 1);
}


double Server::Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2){
    string data=to_string(cp)+"/"+to_string(_r1)+"/"+to_string(_r2)+"/"+to_string(hdd_1)+"/"+to_string(hdd_2);
    message = new char[data.length()];
    vector<char> msg;
    std::copy(data.begin(), data.end(), std::back_inserter(msg));
    for (int i = 0; i < msg.size() && i < data.length(); i++) {
      message[i] = msg[i];
    }
    /*vector<char> msg;
    //assert(msg.size() == sizeof(double));
    memcpy(&cp, &msg[0], std::min(msg.size(), sizeof(double)));*/
    send(newsockfd,&message, sizeof(message),0);
    return 0;
}



void Server::clean(){
  //  memset(msg, 0, PACKET_SIZE);
    close(sockfd);
    close(newsockfd);
}

#endif //UNTITLED4_SERVER_H
