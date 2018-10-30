#include <sys/statvfs.h>
#include <iostream>
#include <stdio.h>

int hdd_1() {
    struct statvfs buf;

    if (statvfs(".", &buf) == -1)
        return -1;
    else {
    int hdd_1=(double)buf.f_bavail * buf.f_frsize/1000000000;
    return hdd_1;
    
 int hdd_2() {
    struct statvfs buf;
    if (statvfs(".", &buf) == -1)
        return -1;
    int hdd_2=(double)buf.f_blocks * buf.f_frsize)/1000000000-hdd_1();
    }
    
    return hdd_2;
}
