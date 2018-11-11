#ifndef UNTITLED4_RAM_H
#define UNTITLED4_RAM_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

static const double gigi = 1024 * 1024 *1024;

static struct sysinfo blya;

double t_ram () {
    sysinfo (&blya);
    double _ram=blya.totalram / gigi;
    return _ram;

}

double f_ram(){
    sysinfo (&blya);
    double _ram=blya.freeram / gigi;
    return _ram;
}

#endif //UNTITLED4_RAM_H
