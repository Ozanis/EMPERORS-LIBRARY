#include "Async.h"


static const uint16_t guest_port = 44300;
static const uint16_t my_port = 44300;
static const char * my_addr = "127.0.0.1";
//static const char * guest_addr = "127.0.0.1";


int main(){
    Server srvr(my_addr, my_port, 5);
    if(srvr.Up() < 0){
       cerr << "Binding error";
       srvr.~Server();
       _exit(0);
   }
    while(1){
        srv.cast();
        srvr.ethr();
    }
    return 0;
}
