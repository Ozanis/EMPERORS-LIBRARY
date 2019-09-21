#ifndef CRYPTSERVERWRAP_H
#define CRYPTSERVERWRAP_H

#include "../SocketWrapper/ServerWrap.h"


class TLS_session : public Connector, public TLS_proto {
    public:
        TLS_session(int sock, uint16_t port);
        bool recv_wr();
        ~TLS_session();
};


TLS_session :: TLS_session(int sock, uint16_t port) : Connector(sock, port), TLS_proto(){
    SSL_CTX_set_ecdh_auto(this->ctx, 1);
    if (SSL_CTX_use_certificate_file(this->ctx, pub_cert, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(this->ctx, pub_key, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
    	exit(EXIT_FAILURE);
    }
    else{
        SSL_set_fd(this->ssl, this->id);
        ShowCerts();
        X509 * cert = SSL_get_peer_certificate(this->ssl);
        long res = SSL_get_verify_result(this->ssl);
        if(!(X509_V_OK == res)) {
            cerr << "handleFailure" << endl;
            return;
        }
        if (SSL_accept(this->ssl) <= 0) ERR_print_errors_fp(stderr);
        else this->ssl = SSL_new(this->ctx);
    }
}


bool TLS_session :: recv_wr(){
    return SSL_read(this->ssl, buffer, BUFF_SIZE) > 0;
}


TLS_session :: ~TLS_session(){
    EVP_cleanup();
}


#endif // CRYPTSERVERWRAP_H

