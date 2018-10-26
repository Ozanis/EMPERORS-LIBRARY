#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int main () {

    const double gigi = 1024 * 1024 *1024;

    struct sysinfo blya;

    while(true){
        sysinfo (&blya);
        printf("total RAM     : %5.2f GB\n", blya.totalram / gigi);
        printf("free RAM      : %5.2f GB\n", blya.freeram / gigi);
        sleep(5);
    }
        return 0;
}
