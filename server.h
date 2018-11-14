#ifndef UNTITLED4_SERVER_H
#define UNTITLED4_SERVER_H

#include <iostream>
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


#define MAXPACKETSIZE 4096

class Server
{
public:
    int sockfd, newsockfd, n, pid;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    char msg[ MAXPACKETSIZE ];
    static string Message;
    void setup(int port);
    double Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2);
    void clean();

private:

};

void Server::setup(int port)
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port=htons(port);
    bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(sockfd, 1);
}


double Server::Send(double cp, double _r1, double _r2, double hdd_1, double hdd_2)
{
    string  message=to_string(cp)+"/"+to_string(_r1)+"/"+to_string(_r2)+"/"+to_string(hdd_1)+"/"+to_string(hdd_2);
    if (message.size()>=MAXPACKETSIZE) return 1;
    int i;
    char *msg = (char*)malloc(sizeof(char)*(message.size()+1));
    for(i=0; i<message.size(); i++){
        msg[i]=message[i];
//        if (i<message.size()) msg=(char*)realloc(msg, (i+1)*sizeof(char));
//        if(i==message.size()) break;
    }
    send(newsockfd,&msg, sizeof(msg),0);
    return 0;
}


void Server::clean()
{
//    Message = "";
    memset(msg, 0, MAXPACKETSIZE);
    close(sockfd);
    close(newsockfd);
//    delete(&sockfd);
//    delete(&newsockfd);
}

#endif //UNTITLED4_SERVER_H
