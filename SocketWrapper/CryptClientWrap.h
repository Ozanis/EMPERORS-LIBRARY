#ifndef CRYPTCLIENTWRAP_H
#define CRYPTCLIENTWRAP_H

#include "../util/crypto.h"
#include "../SocketWrapper/ClientWrap.h"


class cryptClient : public ClientSock, public TLS_proto{
    public:
        cryptClient(const char * from_addr, uint16_t from_port, const char * to_addr, uint16_t to_port);
        ~cryptClient() = default;
        void send_wr(const string & data);
        X509 * cert;
        SSL * ctx;
};


cryptClient :: cryptClient() : ClientSock(from_addr, from_port, to_addr, to_port), TLS_proto(){
    SSL_new(this->ctx);
    SSL_set_tlsext_host_name(this->ctx, this->to_addr);
    SSL_set_fd(this->ssl, this->id);
    if (SSL_connect(this->ssl) < 0) ERR_print_errors_fp(stderr);
    else{
        ShowCerts(this->ssl);
        SSL_write(this->ssl, message, strlen(message));
    }
}


void cryptClient :: send_wr(const string & data){
    size_t bufsize = data.length();
    char * message = new char[bufsize]{0};
    strcpy(message, data.c_str());
    SSL_write(this->ssl, message, bufsize);
//    if(!send(this->id, message, bufsize, 0)) cerr << "Unable to send" << endl;
    delete(message);
}


#endif // CRYPTCLIENTWRAP_H

