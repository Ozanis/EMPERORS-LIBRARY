#ifndef UNTITLED4_RAM_H
#define UNTITLED4_RAM_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <iostream>
#include <math.h>

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
   this->k_r =(double)1024*1024*1024*8*1000;
   this->k_h =(double)1024*1024*1024*1024;
}

memory::~memory(){}

double memory::allHdd(){
    struct statfs dat;
    hdd_tot = (dat.f_bavail * dat.f_frsize)/k_h;
    hdd_tot=floor(hdd_tot*100)/100;
    return hdd_tot;
}

double memory::getHdd(){
    struct statfs dat;
    hdd=((dat.f_blocks * dat.f_frsize -(dat.f_bavail * dat.f_frsize))/k_r);
    hdd=floor(hdd*100)/100;
    return hdd;
}

double memory::allRam(){
    struct sysinfo inf;
    ram_tot=inf.totalram/k_r*1000;
    ram_tot=floor(ram_tot*100)/100;
    return ram_tot;
}

double memory::getRam() {
    struct sysinfo inf;
    ram=(inf.totalram - inf.freeram)/k_r/1000;
    ram=floor(ram*100)/100;
    return ram;
}

#endif //UNTITLED4_RAM_H
