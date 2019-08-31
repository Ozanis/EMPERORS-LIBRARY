#include "asyncServer.h"

static const char * my_addr = "127.0.0.1";
static const uint16_t my_port = 44300;
static const size_t con_limit = 5;


int main(){
    AsyncServer server(my_addr, my_port, con_limit);
    while(1){
        server.asyncCast();
    }
    return 0;
}
