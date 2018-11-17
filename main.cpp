#ifdef __linux__

#include "iostream"
#include "proc.h"
#include "memory.h"
#include "server.h"

int main() {
    double _cp;
    double _r1;
    double _r2;
    double _hdd1;
    double _hdd2;

    vector<CPUData> entries1;
    vector<CPUData> entries2;
    ReadStatsCPU(entries1);
    this_thread::sleep_for(chrono::milliseconds(100));
    ReadStatsCPU(entries2);

    memory meminfo;
    Server cnct;
    cnct.setup();
    do {
        _cp = PrintStats(entries1, entries2);
        _r1 = meminfo.getAllRam();
        _r2 = meminfo.getRam();
        _hdd1 = meminfo.getAllHdd();
        _hdd2 = meminfo.getHdd();
        sleep(5);
        cnct.Send(_cp, _r1, _r2, _hdd1, _hdd2);
        cnct.clean();
    } while (cnct.Send(_cp, _r1, _r2, _hdd1, _hdd2));

    return 0;
}

#elif _WIN32


#else

#endif