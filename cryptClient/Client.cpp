#include "Client.h"
#include "proc.h"
#include "memory.h"


static const uint16_t to_port = 44300;
static const uint16_t from_port = 44300;
static const char * from_addr = "127.0.0.1";
static const char * to_addr = "127.0.0.1";

int main() {
    string data;

    Client clnt(from_addr, from_port, to_addr, to_port);
    if(!clnt.Up()){
        cerr << "Connect error";
        clnt.~Client();
        _exit(0);
    }
    while(gethostbyname(to_addr)){
        data = cpu_num() + get_cpu() + process_mem_usage() + physical_mem_usage();
        cout << data << endl;
        clnt.Send(data);
    }
    return 0;
}
