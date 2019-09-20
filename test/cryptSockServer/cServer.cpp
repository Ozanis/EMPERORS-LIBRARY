#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
//#include <openssl/bio.h>
//#include <openssl/ecdsa.h>
//#include <openssl/ecdh.h>
//#include <openssl/crypto.h>
#include <openssl/pem.h>
#include "CryptServerSock.h" 
//#include <openssl/conf.h>

const char * key_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/ecdhkey.pem";
const char * cert_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/cert.pem";


int create_socket(int port){
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror("Unable to create socket");
	exit(EXIT_FAILURE);
    }
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	perror("Unable to bind");
	exit(EXIT_FAILURE);
    }
    if (listen(s, 1) < 0) {
	perror("Unable to listen");
	exit(EXIT_FAILURE);
    }
    return s;
}

void init_openssl(){ 
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl(){ EVP_cleanup(); }

SSL_CTX *create_context(){
    const SSL_METHOD * method;
    SSL_CTX *ctx;
    method = SSLv23_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
	perror("Unable to create SSL context");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
    return ctx;
}

void configure_context(SSL_CTX *ctx){
    SSL_CTX_set_ecdh_auto(ctx, 1);
    if (SSL_CTX_use_certificate_file(ctx, cert_pem, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, key_pem, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
}


int main(){
    SSL_CTX *ctx;
    init_openssl();
    ctx = create_context();
    configure_context(ctx);
    int sock = create_socket(44300);
    int client;
    SSL *ssl;

    while(1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        const char reply[] = "test\n";
        cout << "Incoming connection" << endl;
        client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);
        X509 * cert = SSL_get_peer_certificate(ssl);
//        if(NULL == cert) cerr << "handleFailure()";
//        if(cert) X509_free(cert); 
        long res = SSL_get_verify_result(ssl);
        if(!(X509_V_OK == res))cerr << "handleFailure";
        if (SSL_accept(ssl) <= 0) ERR_print_errors_fp(stderr);
        else SSL_write(ssl, reply, strlen(reply));
    }
    SSL_free(ssl);
    close(client);
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
    return 0;
}


