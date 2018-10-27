#include <sys/statvfs.h>
#include <iostream>
#include <stdio.h>

int main() {
    int fd;
    struct statvfs buf;

    if (statvfs(".", &buf) == -1)
        perror("FUCK, sys/statvfs.h DOESN'T WORK, VALERA BLYAT!!!");
    else {
        printf("%.0f GB free of %.0f GB\n", (double)buf.f_bavail * buf.f_frsize/1000000000, ((double)buf.f_blocks * buf.f_frsize)/1000000000);
    }
    return 0;
}