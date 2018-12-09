#ifdef __linux__
#include "server.h"
#include "proc.h"
#include "memory.h"

int main() {
    string data;
    Server cnct;
    cnct.set_up();

    cnct.conn();
    data = cpu_num()+get_cpu()+process_mem_usage()+physical_mem_usage();
    cnct.Send(data);
    sleep(5);

    cnct.off();
    return 0;
}

#elif _WIN32


#else

#endif