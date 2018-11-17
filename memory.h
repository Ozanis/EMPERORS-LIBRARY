#ifndef UNTITLED4_RAM_H
#define UNTITLED4_RAM_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

class memory {
public:
    double gigi=1000000000;
    double ram_tot;
    double ram;
    double hdd_tot;
    double hdd;

    memory();
    ~memory();

    double getRam();
    double getHdd();
    double getAllRam();
    double getAllHdd();
};

memory::memory(){}

memory::~memory(){}

double memory::getAllHdd(){
    struct statvfs inf;
    statvfs(".", &inf);
    hdd_tot=(inf.f_blocks * inf.f_frsize)/gigi;
    return floor(hdd_tot*100)/100;
}

double memory::getHdd(){
    struct statvfs inf;
    statvfs(".", &inf);
    hdd=(getAllHdd()- inf.f_bavail * inf.f_frsize/gigi);
    return floor(hdd*100)/100;
}

double memory::getAllRam(){
    struct sysinfo inf;
    ram_tot = inf.totalram/gigi/10000;
    return floor(ram_tot*100)/100;
}

double memory::getRam() {
    struct sysinfo inf;
    ram = (inf.totalram-inf.freeram)/gigi/gigi/10;
    return floor(ram*100)/100;
}

#endif //UNTITLED4_RAM_H
