#include "async.h"


static const uint16_t my_port = 44300;
static const char * my_addr = "127.0.0.1";
static const size_t con_limit = 5;

//static const char * guest_addr = "127.0.0.1";


int main(){
    Server server(my_addr, my_port, con_limit);
    while(1){
        server.cast();
    }
    return 0;
}
