#ifdef __linux__

#include "server.h"
#include "proc.h"
#include "memory.h"

int main() {
    string data = cpu_num()+get_cpu()+process_mem_usage()+physical_mem_usage();
    Server cnct;
    cnct.set_up();
    cnct.Send(data);
    cnct.off();
    sleep(5);

    return 0;
}

#elif _WIN32


#else

#endif