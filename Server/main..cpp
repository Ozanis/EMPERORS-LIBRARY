#include "Server.h"
#include "dcmprs.h"

int main(){
    while (gethostbyname("127.0.0.1")) {
        auto *srvr = new Server;
        clnt->set_up();
        clnt->Rec();
        clnt->off();
        sleep(5);
    }
    return 0;
}
