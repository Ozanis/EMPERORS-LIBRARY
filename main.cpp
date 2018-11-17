#ifdef __linux__

#include "iostream"
#include "proc.h"
#include "memory.h"
#include "server.h"

int main() {
    uint16_t PORT = 4430;
    vector<CPUData> entries1;
    vector<CPUData> entries2;
    ReadStatsCPU(entries1);
    this_thread::sleep_for(chrono::milliseconds(100));
    ReadStatsCPU(entries2);

    memory meminfo;
    double cp = PrintStats(entries1, entries2);
    double _r1 = meminfo.getAllRam();
    double _r2 = meminfo.getRam();
    double _hdd1 = meminfo.getAllHdd();
    double _hdd2 = meminfo.getHdd();

    Server cnct;
    cnct.setup();
    while (true) {
    cnct.Send(cp, _r1, _r2, _hdd1, _hdd2);
}
    cnct.clean();

    return 0;
}

#elif _WIN32



#else

#endif