#ifndef CRYPTSERVERWRAP_H
#define CRYPTSERVERWRAP_H


#include "../SocketWrapper/ServerWrap.h"
#include "openssl/evp.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/pem.h"


const char * key_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/ecdhkey.pem";
const char * cert_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/cert.pem";


class cryptServer : public ServerSock{
    public:
        cryptServer(const char * addr, uint16_t port) : ServerSock(addr, port) : ServerSock(addr, port){};
        void session();
        ~cryptServer();
    private:
        SSL_CTX * ctx;
        SSL *ssl;

};


void cryprServer :: session(int client){
    SSL_set_fd(this->ssl, client);
    ShowCerts();
    X509 * cert = SSL_get_peer_certificate(this->ssl);
    if(!(X509_V_OK == res)) {
        cerr << "handleFailure";
        return;
    }
    if (SSL_accept(this->ssl) <= 0) ERR_print_errors_fp(stderr);
}


cryptServer :: cryptServer(const char * addr, uint16_t port) : ServerSock(addr, port){
    init_openssl();
    this->ctx = create_context();
    configure_context(this->ctx);
    this->ssl = SSL_new(this->ctx);
}


cryprServer :: ~cryptServer(){
    SSL_free(this->ssl);
    close(this->client);
    close(this->sock);
    SSL_CTX_free(this->ctx);
    EVP_cleanup();
    cleanup_openssl();
}


#endif // CRYPTSERVERWRAP_H

