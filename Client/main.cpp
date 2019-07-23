#include "Client.h"
#include "proc.h"
#include "memory.h"

int main() {
    string data;
    Client clnt;
    clnt.set_up();

    clnt.conn();
    data = cpu_num()+get_cpu()+process_mem_usage()+physical_mem_usage();
    clnt.Send(data);
    sleep(5);

    return 0;
}

#endif