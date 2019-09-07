#ifndef CLIENTWRAP_H 
#define CLIENTWRAP_H 


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include "unistd.h"
#include <iostream>
#include <cstring>


using std :: cerr;
using std :: cout;
using std :: endl;
using std :: string;


class ClientSock{
    public:
        ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
        ~ClientSock();
        void send_wr(const string & data);
        int id = 0;
        struct sockaddr_in Client;
        struct sockaddr_in Server;
};


ClientSock :: ClientSock(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port){
   this->id = socket(AF_INET, SOCK_STREAM, 0);
   if(!this->id){
       cerr << "Error client socket creation" << endl;
       exit(0);
       }
    this->Client.sin_family = AF_INET;
    this->Client.sin_addr.s_addr = inet_addr(from_addr);
    this->Client.sin_port = htons(from_port);

    this->Server.sin_family = AF_INET;
    this->Server.sin_addr.s_addr = inet_addr(to_addr);
    this->Server.sin_port = htons(to_port);

    if(connect(this->id, (struct sockaddr *) & this->Server, sizeof(this->Server)) != 0){
        cerr << "Connection error" << endl;
        exit(0);
    }
    else cout << "Connection established" << endl;
}


void ClientSock :: send_wr(const string & data){
    size_t bufsize = data.length();
    char * message = new char[bufsize]{0};
    strcpy(message, data.c_str());
    if(!send(this->id, message, bufsize, 0)) cerr << "Unable to send" << endl;
    delete(message);
}


ClientSock :: ~ClientSock(){
    close(this->id);
}

#endif // CLIENTWRAP_H 
