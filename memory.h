#ifndef UNTITLED2_HDD_H
#define UNTITLED2_HDD_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <fstream>

struct statvfs inf;

void physical_mem_usage(double &_hdd1, double &_hdd2){
    statvfs(".", &inf);

    _hdd1=(inf.f_blocks * inf.f_frsize);
    _hdd2=(_hdd1 - inf.f_bavail * inf.f_frsize);

    _hdd1/=1000000000.0;
    _hdd2/=1000000000.0;

    _hdd1 =floor(_hdd1*100)/100;
    _hdd2 = floor(_hdd2*100)/100;
}

void process_mem_usage(double& vm_usage, double& resident_set){
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
            >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
            >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0 / 1024.0;
    vm_usage = floor(vm_usage*100)/100;
    resident_set = (rss * page_size_kb)/1024.0;
    resident_set=floor(resident_set*100)/100;
}

#endif //UNTITLED2_HDD_H
