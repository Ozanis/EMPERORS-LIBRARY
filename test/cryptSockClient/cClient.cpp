const char * pub_key = "/home/max/Projects/TelemetryAgent/cryptkeys/dhkey.pem";
const char * pub_cert = "/home/max/Projects/TelemetryAgent/cryptkeys/cert.pem";


#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 
#define FAIL    -1

void init_openssl_library(){
    SSL_library_init();
    SSL_load_error_strings();
    }

int OpenConnection(const char *hostname, int port){
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(hostname);
    cout << "Connection init" << endl;
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    cout << "Connected"  << endl;
    return sd;
}


SSL_CTX* InitCTX(void){
    auto * method = SSLv23_method();  /* Create new client-method instance */
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 

void ShowCerts(SSL* ssl){
    X509 *cert;
    char *line;
    cout << "getting peer cert" << endl; 
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}
 

void verify_callback(X509_STORE_CTX* x509_ctx, SSL_CTX * ctx){
    int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
    int err = X509_STORE_CTX_get_error(x509_ctx);
    X509 * cert = X509_STORE_CTX_get_current_cert(x509_ctx);
    X509_NAME* iname = cert ? X509_get_issuer_name(cert) : NULL;
    X509_NAME* sname = cert ? X509_get_subject_name(cert) : NULL;
//    print_cn_name("Issuer (cn)", iname);
//    print_cn_name("Subject (cn)", sname);
    if(depth == 0) {
        /* If depth is 0, its the server's certificate. Print the SANs too */
//        print_san_name("Subject (san)", cert);
    }
//    return preverify;
}

int main(int count, char *strings[]){
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
	char acClientRequest[1024] ={0};
    int bytes, portnum = 44300;
    char * hostname = "127.0.0.1";
    SSL_library_init(); 
    ctx = InitCTX();
    server = OpenConnection(hostname, portnum);
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_tlsext_host_name(ssl, hostname);
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else    {
//        verify_callback(); 
//        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
        ShowCerts(ssl);        /* get any certs */
        SSL_write(ssl,acClientRequest, strlen(acClientRequest));   /* encrypt & send message */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
        buf[bytes] = 0;
        printf("Received: \"%s\"\n", buf);
        SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}



/*
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


void init_openssl_library(void){
  (void)SSL_library_init();

  SSL_load_error_strings();


  OPENSSL_config(nullptr);
}


int verify_callback(int preverify, X509_STORE_CTX* x509_ctx)
{
    int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
    int err = X509_STORE_CTX_get_error(x509_ctx);

    X509* cert = X509_STORE_CTX_get_current_cert(x509_ctx);
    X509_NAME* iname = cert ? X509_get_issuer_name(cert) : nullptr;
    X509_NAME* sname = cert ? X509_get_subject_name(cert) : nullptr;

    print_cn_name("Issuer (cn)", iname);
    print_cn_name("Subject (cn)", sname);

    if(depth == 0) {
        print_san_name("Subject (san)", cert);
    }

    return preverify;
}

#define HOST_NAME "www.random.org"
#define HOST_PORT "443"
#define HOST_RESOURCE "/cgi-bin/randbyte?nbytes=32&format=h"

#if (SSLEAY_VERSION_NUMBER >= 0x0907000L)
# include <openssl/conf.h>
#endif

int main(){

    long res = 1;

    SSL_CTX* ctx = nullptr;
    BIO *web = nullptr, *out = nullptr;
    SSL *ssl = nullptr;

    init_openssl_library();

    const SSL_METHOD* method = SSLv23_method();
    if(!(nullptr != method)) handleFailure();

    ctx = SSL_CTX_new(method);
    if(!(ctx != nullptr)) handleFailure();

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);

    SSL_CTX_set_verify_depth(ctx, 4);

    const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
    SSL_CTX_set_options(ctx, flags);

    res = SSL_CTX_load_verify_locations(ctx, "random-org-chain.pem", nullptr);
    if(!(1 == res)) handleFailure();

    web = BIO_new_ssl_connect(ctx);
    if(!(web != nullptr)) handleFailure();

    res = BIO_set_conn_hostname(web, HOST_NAME ":" HOST_PORT);
    if(!(1 == res)) handleFailure();

    BIO_get_ssl(web, &ssl);
    if(!(ssl != nullptr)) handleFailure();

    const char* const PREFERRED_CIPHERS = "HIGH:!anullptr:!kRSA:!PSK:!SRP:!MD5:!RC4";
    res = SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);
    if(!(1 == res)) handleFailure();
    
    res = SSL_set_tlsext_host_name(ssl, HOST_NAME);
    if(!(1 == res)) handleFailure();

    out = BIO_new_fp(stdout, BIO_NOCLOSE);
    if(!(nullptr != out)) handleFailure();

    res = BIO_do_connect(web);
    if(!(1 == res)) handleFailure();

    res = BIO_do_handshake(web);
    if(!(1 == res)) handleFailure();

    X509* cert = SSL_get_peer_certificate(ssl);
    if(cert) { X509_free(cert); }
    if(nullptr == cert) handleFailure();

    res = SSL_get_verify_result(ssl);
    if(!(X509_V_OK == res)) handleFailure();


BIO_puts(web, "GET " HOST_RESOURCE " HTTP/1.1\r\n"
              "Host: " HOST_NAME "\r\n"
              "Connection: close\r\n\r\n");
BIO_puts(out, "\n");

int len = 0;
do
{
  char buff[1536] = {0};
  len = BIO_read(web, buff, sizeof(buff));

  if(len > 0)
    BIO_write(out, buff, len);

} while (len > 0 || BIO_should_retry(web));

if(out)
  BIO_free(out);

if(web != nullptr)
  BIO_free_all(web);

if(nullptr != ctx)
  SSL_CTX_free(ctx);

  return 0;
}

*/
