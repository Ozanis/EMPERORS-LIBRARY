#include "iostream"
#include "proc.h"
#include "ram.h"
#include "server.h"
#include "hdd.h"

int main(){
    vector<CPUData> entries1;
    vector<CPUData> entries2;
    ReadStatsCPU(entries1);
    this_thread::sleep_for(chrono::milliseconds(100));
    ReadStatsCPU(entries2);
    int cp = PrintStats(entries1, entries2);
    int _r1 = t_ram();
    int _r2 = f_ram();
    int _hdd1= hdd_1();
    int _hdd2=hdd_2();
    server(cp, _r1, _r2, _hdd1, _hdd2);
    while (server(cp, _r1, _r2)==0) {
        sleep(5);
        server(cp, _r1, _r2);
    }
    return 0;
}
