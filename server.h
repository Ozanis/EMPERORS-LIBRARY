#ifndef UNTITLED4_SERVER_H
#define UNTITLED4_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

int sock;
struct sockaddr_in addr;

using namespace std;

int server(int cp, int _r1, int _r2, int hdd_1, int hdd_2) {
    int BUFFER_MAX_SIZE = 25;
    string  message=to_string(cp)+"/"+to_string(_r1)+"/"+to_string(_r2)+"/"+to_string(hdd_1)+"/"+to_string(hdd_2);
    if (message.size()>=BUFFER_MAX_SIZE) return 1;
    int i;
    char *msg = (char*)malloc(sizeof(char));
    for(i=1; i<=message.size(); i++){
        msg[i]=message[i];
        if (i<message.size()) msg=(char*)realloc(msg, (i+1)*sizeof(char));
        if(i==message.size()) break;
    }

    char buf[sizeof(msg)];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(4430);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    send(sock, msg, sizeof(msg), 0);
    //recv(sock, buf, sizeof(msg), 0);

   // printf(buf);
    close(sock);

    return 0;
}

#endif //UNTITLED4_SERVER_H
