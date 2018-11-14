#include "iostream"
#include "proc.h"
#include "memory.h"
#include "server.h"

#define PORT 4430

Server server;

int main(){
    vector<CPUData> entries1;
    vector<CPUData> entries2;
    ReadStatsCPU(entries1);
    this_thread::sleep_for(chrono::milliseconds(100));
    ReadStatsCPU(entries2);
    double cp = PrintStats(entries1, entries2);
    memory meminfo;
    cout << meminfo.allRam()<<endl;
    cout<< meminfo.getRam()<<endl;
    cout << meminfo.allHdd()<<endl;
   cout << meminfo.getHdd()<< endl;

    server.setup(PORT);
    /*
    while (1) {
        sleep(5);
        server.Send(cp, _r1, _r2, _hdd1, _hdd2);
        server.clean();
    }
     */
    return 0;
}
