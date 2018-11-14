#ifndef UNTITLED4_RAM_H
#define UNTITLED4_RAM_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class memory{

public:
    double k_r;
    double k_h;
    double ram_tot;
    double ram;
    double hdd_tot;
    double hdd;

    memory();
    ~memory();
    double getRam();
    double getHdd();
    double allRam();
    double allHdd();
};

memory::memory(){
   this->k_r =(double)1024*1024*1024*8;
    this->k_h =(double)1000*1000*1000;
}

memory::~memory(){}

double memory::allHdd(){
    struct statfs dat;
    hdd_tot = (dat.f_bavail * dat.f_frsize)/k_h;
    return hdd_tot;
}

double memory::getHdd(){
    struct statfs dat;
    hdd=(dat.f_blocks * dat.f_frsize -(dat.f_bavail * dat.f_frsize))/k_h;
    return hdd;
}

double memory::allRam(){
    struct sysinfo inf;
    ram_tot=inf.totalram/k_r;
    return ram_tot;
}

double memory::getRam() {
    struct sysinfo inf;
    ram=(inf.totalram - inf.freeram) /k_r;
    return ram;
}

#endif //UNTITLED4_RAM_H
