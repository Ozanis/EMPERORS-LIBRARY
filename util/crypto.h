#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/ecdh.h>
#include <openssl/pem.h>


const char * pub_key = "/home/max/Projects/TelemetryAgent/cryptkeys/dhkey.pem";
const char * pub_cert = "/home/max/Projects/TelemetryAgent/cryptkeys/cert.pem";


void init_openssl_library(){
    SSL_library_init();
    SSL_load_error_strings();
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


/*
SSL_CTX* InitCTX(void){
    auto * method = SSLv23_method(); 
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();   
    ctx = SSL_CTX_new(method); 
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
*/




/*
int verify_callback_server(int ok, X509_STORE_CTX *ctx)
{
    printf("verify_callback_server.\n");
    return ok;
}

int SSL_CTX_use_PrivateKey_file_pass(SSL_CTX *ctx, char *filename, char *pass)
{
    EVP_PKEY *pkey = NULL;
    BIO *key = NULL;
    key = BIO_new(BIO_s_file());

    BIO_read_filename(key, filename);
    pkey = PEM_read_bio_PrivateKey(key, NULL, NULL, pass);

    if (pkey==NULL)
    {
        printf("PEM_read_bio_PrivateKey failed");
        return -1;
    }

    if (SSL_CTX_use_PrivateKey(ctx, pkey)<=0)
    {
        printf("SSL_CTX_use_PrivateKey err.\n");
        return -1;
    }

    BIO_free(key);
    return 1;
}

int main()
{
    int ret;

    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();

    const SSL_METHOD *meth = SSLv23_server_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);
    if (!ctx)
    {
        printf("SSL_CTX_new failed. ctx=%d\n", ctx);
        return -1;
    }

    if ((!SSL_CTX_load_verify_locations(ctx, CAFILE, NULL))||(!SSL_CTX_set_default_verify_paths(ctx)))
    {
        printf("error.\n");
        return -1;
    }

    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM)<=0)
    {

        printf("SSL_CTX_use_certificate_file failed.\n");
        return -1;
    }

    if (SSL_CTX_use_PrivateKey_file_pass(ctx,KEYF,NULL)<=0)
    {

        printf("SSL_CTX_use_PrivateKey_file_pass failed.\n");
        return -1;
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        printf("SSL_CTX_check_private_key failed.\n");
        return -1;
    }

    int s_server_verify = SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_CLIENT_ONCE;
    SSL_CTX_set_verify(ctx, s_server_verify, verify_callback_server);
    SSL_CTX_set_client_CA_list(ctx, SSL_load_client_CA_file(CAFILE));

    int s_handle = socket(AF_INET, SOCK_STREAM, 6);
    if (s_handle<0)
    {
        printf("create socket failed.\n");
        return -1;
    }

    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = htons(INADDR_LOOPBACK);
    service.sin_port = htons(1111);

    if (bind(s_handle, (const struct sockaddr *)&service, sizeof(service))==-1)
    {
        printf("bind failed.\n");
        close(s_handle);
        return -1;
    }

    if (listen(s_handle, 1)==-1)
    {
        printf("listen failed.\n");
        return -1;
    }

    while(1)
    {
        int session_socket = accept(s_handle, NULL, NULL);
        if(session_socket < 0)
        {
            printf("accept failed.\n");
            return -1;
        }
        SSL *ssl = SSL_new(ctx);
        err = SSL_set_fd(ssl, session_socket);
        if (err < 0)
        {
            printf("SSL_set_fd failed.\n");
            return -1;
        }

        err = SSL_accept(ssl);
        if (err < 0)
        {
            printf("SSL_accept failed. errstr=%s\n", SSL_state_string_long(ssl));
            return -1;
        }

        printf("SSL_connection using %s\n", SSL_get_cipher(ssl));

        X509 *client_cert = SSL_get_peer_certificate(ssl);
        if (client_cert != NULL)
        {
            printf("client cerificate:\n");
            char *str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
            if (str&&(*str))
                printf("\tsubject:%s\n", str);
            OPENSSL_free(str);

            str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
            if(str&&(*str))
                printf("\t issuer:%s\n", str);
            OPENSSL_free(str);

            X509_free(client_cert);
        }else
            printf("Client does not have certificate.\n");
        const char *cipher_name = SSL_get_cipher_name(ssl);
        if (cipher_name && *cipher_name)
        {
            printf("SSL_get_cipher_name %s.\n", cipher_name);
            return -1;
        }

        char buf[1024];
        memset(buf, 0, 1024);
        err = SSL_read(ssl, buf, sizeof(buf)-1);
        if (err<0)
        {
            printf("SSL_read failed.\n");
            return err;
        }
        printf("get: %s\n", buf);

        SSL_free(ssl);
        close(s_handle);
    }

    SSL_CTX_free(ctx);
    return 0;
}*/
