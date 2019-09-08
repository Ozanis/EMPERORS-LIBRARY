#include "asyncServer.h"


using std :: thread;
using std :: mutex;


static const char * my_addr = "127.0.0.1";
static const uint16_t my_port = 44301;


void listen_cycle(AsyncServer * server){
    mutex mtx;
    while(1){
        if (server->listen_wr()){
            cout << "Adding connection" << endl;
            mtx.lock();
            server->add_client();
            mtx.unlock(); 
        }
    }
}


int main(){
    auto Server = new AsyncServer(my_addr, my_port);
    thread handle_conns_async(listen_cycle, Server);
    handle_conns_async.detach();
    Server->asyncCast();
    return 0;
}
