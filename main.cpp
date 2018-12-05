#ifdef __linux__

#include "iostream"
#include "proc.h"
#include "memory.h"
#include "server.h"
#include "math.h"

using namespace std;

int main() {
    double _cp, _hdd1, _hdd2, _r1, _r2;
    long _ncp = sysconf(_SC_NPROCESSORS_ONLN);
    string data;
    Server cnct;
    if (!cnct.set_up()) return 1;
    else{
        do {
            vector<CPUData> entries1;
            vector<CPUData> entries2;

            ReadStatsCPU(entries1);
            this_thread::sleep_for(chrono::milliseconds(200));
            ReadStatsCPU(entries2);
            PrintStats(entries1, entries2, _cp);

            process_mem_usage(_r1, _r2);
            physical_mem_usage(_hdd1, _hdd2);
            data=to_string(_cp)+"/"+to_string(_ncp)+"/"+to_string(_r1)+"/"+to_string(_r2)+"/"+to_string(_hdd1)+"/"+to_string(_hdd2);
            sleep(4);
        } while (!cnct.Send(data));
    } return 0;
}

#elif _WIN32


#else

#endif