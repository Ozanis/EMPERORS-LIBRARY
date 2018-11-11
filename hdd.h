#ifndef UNTITLED4_HDD_H
#define UNTITLED4_HDD_H

#include <sys/statvfs.h>
#include <iostream>
#include <stdio.h>

double hdd_1() {
    struct statvfs buf;
    if (statvfs(".", &buf) == -1) return -1;
    else {
        double hdd = (double) buf.f_bavail * buf.f_frsize / 1000000000;
        return hdd;
    }
}

double hdd_2() {
    struct statvfs buf;
    if (statvfs(".", &buf) == -1) return -1;
    else{
        double hdd=(double)buf.f_blocks * buf.f_frsize/1000000000-hdd_1();
        return hdd;
    }
}


#endif //UNTITLED4_HDD_H
