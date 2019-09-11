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

const char * key_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/X9_62-key.pem";
const char * cert_pem = "/home/max/Projects/TelemetryAgent/Service/cryptkeys/X9_62-cert1.pem";

/*
#define FAIL    -1
 
 
 
// Create the SSL socket and intialize the socket address structure
int OpenListener(int port)
{   int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
 
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

SSL_CTX* InitServerCTX(void){
     auto method = TLSv1_2_server_method()

    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();   
    ctx = SSL_CTX_new(method); 
    if ( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 
void LoadCertificates(SSL_CTX* ctx, const  char * CertFile, const  char * KeyFile){
    if (!SSL_CTX_check_private_key(ctx)){
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }

    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
}


void ShowCerts(SSL* ssl){
   X509 *cert;
   char *line;
 
    cert = SSL_get_peer_certificate(ssl);
    if ( cert != nullptr )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else printf("No certificates.\n");
}
 
void Servlet(SSL* ssl)
{   char buf[1024] = {0};
   
    int sd, bytes;
    const char* ServerResponse="<\Body>\
	                             <Name>aticleworld.com</Name>\
								 <year>1.5</year>\
								 <BlogType>Embedede and c\c++<\BlogType>\
								 <Author>amlendra<Author>\
								 <\Body>";
								 
	const char *cpValidMessage = "<Body>\
	                             <UserName>aticle<UserName>\
								 <Password>123<Password>\
								 <\Body>";							 
 
    if ( SSL_accept(ssl) == FAIL )  
        ERR_print_errors_fp(stderr);
    else
    {
        ShowCerts(ssl);       
        bytes = SSL_read(ssl, buf, sizeof(buf));

		
		printf("Client msg: \"%s\"\n", buf);
		
        if ( bytes > 0 )
        {
		    if(strcmp(cpValidMessage,buf) == 0)
			{
				SSL_write(ssl, ServerResponse, strlen(ServerResponse)); 
			}
			else
			{
				SSL_write(ssl, "Invalid Message", strlen("Invalid Message"));
			}
        }
        else
		{
			ERR_print_errors_fp(stderr);
		}
            
    }
    sd = SSL_get_fd(ssl);     
    SSL_free(ssl);        
    close(sd);         
}
 
int main(int count, char *Argc[])
{   SSL_CTX *ctx;
    int server;
    int portnum;
 
 
 //Only root user have the permsion to run the server
if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }
    if ( count != 2 )
    {
        printf("Usage: %s <portnum>\n", Argc[0]);
        exit(0);
    }
	
    SSL_library_init();
 
    portnum = 44300;
    ctx = InitServerCTX();      
    LoadCertificates(ctx, cert_pem, key_pem); 
    server = OpenListener(portnum);    
    while (1)
    {   struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        SSL *ssl;
 
        int client = accept(server, (struct sockaddr*)&addr, &len);  
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        ssl = SSL_new(ctx);            
        SSL_set_fd(ssl, client);     
        Servlet(ssl);       
    }
    close(server);         
    SSL_CTX_free(ctx);    
}
*/


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
//    method = SSLv23_server_method();
    method = TLSv1_2_server_method();
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
