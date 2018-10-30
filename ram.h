#ifndef UNTITLED4_RAM_H
#define UNTITLED4_RAM_H

#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

static const double gigi = 1024 * 1024 *1024;

static struct sysinfo blya;

int t_ram () {
    sysinfo (&blya);
    double _ram=blya.totalram / gigi;
    int ram = (int)_ram;
    return ram;

}

int f_ram(){
    sysinfo (&blya);
    double _ram=blya.freeram / gigi;
    int ram = (int)_ram;
    return ram;
}

#endif //UNTITLED4_RAM_H
