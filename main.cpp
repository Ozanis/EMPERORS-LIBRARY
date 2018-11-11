#include "iostream"
#include "proc.h"
#include "ram.h"
#include "server.h"
#include "hdd.h"

#define PORT 4430

Server server;

int main(){
    vector<CPUData> entries1;
    vector<CPUData> entries2;
    ReadStatsCPU(entries1);
    this_thread::sleep_for(chrono::milliseconds(100));
    ReadStatsCPU(entries2);
    double cp = PrintStats(entries1, entries2);
    double _r1 = t_ram();
    double _r2 = f_ram();
    double _hdd1= hdd_1();
    double _hdd2= hdd_2();
    
    server.setup(PORT);
    while (1) {
        sleep(5);
        server.Send(cp, _r1, _r2, _hdd1, _hdd2);
        server.clean();
    }
    return 0;
}
