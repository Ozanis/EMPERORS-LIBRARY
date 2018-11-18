#ifdef __linux__

#include "iostream"
#include "proc.h"
#include "memory.h"
#include "server.h"
#include "math.h"

using namespace std;

int main() {
    double _cp, _hdd1, _hdd2, _r1, _r2;

    Server cnct;
    cnct.setup();
    do {

        vector<CPUData> entries1;
        vector<CPUData> entries2;

        ReadStatsCPU(entries1);
        this_thread::sleep_for(chrono::milliseconds(200));
        ReadStatsCPU(entries2);
        PrintStats(entries1, entries2, _cp);

        process_mem_usage(_r1, _r2);
        physical_mem_usage(_hdd1, _hdd2);
        cout<< _cp << endl << _r1 << endl << _r2 << endl << _hdd1 << endl << _hdd2 << endl;

        cnct.Send(_cp, _r1, _r2, _hdd1, _hdd2);
        cnct.clean();

        sleep(4);
    } while (cnct.Send(_cp, _r1, _r2, _hdd1, _hdd2));

    return 0;
}

#elif _WIN32


#else

#endif